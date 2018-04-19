/*
 * SIM900.cpp
 *
 *  Created on: Apr 4, 2018
 *      Author: erki
 */

#include "SIM900.h"
#include <Arduino.h>

void powerUp()
{
	// if the GSM shield is turned off, this sequence should start it up
	pinMode(9, OUTPUT);
	digitalWrite(9,LOW);
	delay(1000);
	digitalWrite(9,HIGH);
	delay(2000);
	digitalWrite(9,LOW);
	delay(3000);
}

SIM900::SIM900(SoftwareSerial * serial) { //configure software serial port
	this->gsm_modem = serial;
	this->gsm_modem->begin(19200);  // for GSM shield
}

bool SIM900::initialize() {
	// first test that GSM shield is active
	char retry_count = 5;
	while (retry_count > 0) {
		if (this->testGSM() == true) {
			this->initialized = true;
			if (this->command(F("ATE0")) == false){
				//turn echo off
				continue;
			}
			return true;
		} else {
			retry_count--;
			powerUp(); // SIM900 did not respond
		}
	}
	// initialization failed (maybe no GSM shield attached)
	return false;
}

bool SIM900::testGSM()
{
	// test if GSM shield is active
	this->gsm_modem->println(F("AT+CPAS")); //here we check SIM900 activity
	int char_count=0;
	char inchar;
	for (int k = 0; k < 5000; k++) {
		if(this->gsm_modem->available() >0)
		{
			inchar=this->gsm_modem->read();
			char_count++;
		}
	}
	if (char_count == 0) {
		return false;
	} else {
		return true;
	}
}

bool SIM900::getTimeString(char* buf, const int buf_size) {
	// AT+CCLK? returns string +CCLK: "18/04/03,09:10:26
	char stringone[30] = "";
	char inchar;
	this->gsm_modem->flush();
	this->gsm_modem->println(F("AT+CCLK?"));
	delay(100);
	int index = 0;
	while(this->gsm_modem->available() && index < 30 - 1){
		inchar = this->gsm_modem->read();
		stringone[index] = inchar;
		index++;
	}
	stringone[index] = '\0';

	char *pch = strstr(stringone, "+CCLK:");

	if(pch)
	{
	    snprintf(buf, buf_size, "%s", pch + 8);
	    return true;
	} else {
		Serial.println(F("CCLK NOT FOUND"));
		return false;
	}
}

// converts two characters from date string into a two digit decimal value
int dateToDecimal(char * date){
	int value = ((date[0] - '0') * 10) +  (date[1] - '0');
	return value;
}

time_t SIM900::getCurrentTime() {
	if (this->initialized == false) {
		return 0;
	}

	char timebuf[20];
	if (this->getTimeString(timebuf, 20)){
		int year = dateToDecimal(&timebuf[0]);
		int month = dateToDecimal(&timebuf[3]) ;
		int day = dateToDecimal(&timebuf[6]) ;
		int hour = dateToDecimal(&timebuf[9]) ;
		int minute = dateToDecimal(&timebuf[12]) ;
		int second = dateToDecimal(&timebuf[15]) ;
		setTime(hour,minute,second,day,month,year); // set arduino time library time so correct time_t object will be returned with now()
		return now();
	} else {
		return 0;
	}
}

bool SIM900::connectToGPRS(){
	this->gsm_modem->flush();
	// first test current connection status
	if (this->command(F("AT+CIFSR"), "ERROR", 1000) == false){
		// got IP instead of ERROR, connect to GPRS not needed!
		this->gprs_connected = true;
		return true;
	}
	this->command(F("AT+CGATT=1")); // attach to GPRS Service
	delay(3000);
	this->command(F("AT+CIPMUX=0")); //We only want a single IP connection
	this->command(F("AT+CIPMODE=0")); //Selecting Normal Mode
	this->command(F("AT+CGDCONT=1,\"IP\",\"internet.emt.ee\""));
	delay(2000);
	this->command(F("AT+CSTT=\"internet.emt.ee\",\"\",\"\""), 3000); //Start Task and set Access Point Name
	delay(1000);
	if (this->command(F("AT+CIICR"), 15000) == false) { // create wireless connection to GPRS (should get IP after this)
		this->gprs_connected = false;
		return false;
	}
	this->gprs_connected = true;
	return true;
}

void SIM900::printIP() {
	this->gsm_modem->println(F("AT+CIFSR"));
	delay(1000);
	this->printResponse("IP:");
}

bool SIM900::connectToServer() {
	//this->gsm_modem->println(F("AT+CIPSTART=\"TCP\",\"erki.pelltech.eu\",\"8234\""));
	this->gsm_modem->println(F("AT+CIPSTART=\"TCP\",\"kasvuhoone-echo.herokuapp.com\",\"80\""));
	delay(5000);
	char response_buf[20] = "";
	this->readResponseToBuff(response_buf, 20);
	Serial.println(response_buf);
	if (strncmp(response_buf, "OK", 20) == 0) {
		this->readResponseToBuff(response_buf, 20);
		Serial.println(response_buf);
		if (strncmp(response_buf, "CONNECT OK", 20) == 0) {
			return true;
		} else if (strncmp(response_buf, "STATE: TCP CLOSED", 20) == 0) {
			return false;
		} else {
			return false;
		}
	} else if (strncmp(response_buf, "ERROR", 20) == 0) {
		this->readResponseToBuff(response_buf, 20);
		Serial.println(response_buf);
		if (strncmp(response_buf, "ALREADY CONNECT", 20) == 0) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool SIM900::disconnectFromServer() {
	if (this->command(F("AT+CIPCLOSE"), "CLOSE OK", 5000)) {
		this->server_connected = false;
		return true;
	} else {
		return false;
	}
}

void SIM900::writeJsonToServer(JsonObject* json) {
	Serial.print(F("Msg to server: "));
	json->printTo(Serial);
	if (this->command(F("AT+CIPSEND"), "> ") == true){

		this->gsm_modem->flush();
		this->gsm_modem->print(F("PUT /"));
		this->gsm_modem->println(F(" HTTP/1.1"));
		this->gsm_modem->println(F("Host: kasvuhoone-echo.herokuapp.com"));
		this->gsm_modem->println(F("Connection: close"));
		this->gsm_modem->println(F("User-Agent: Kasvuhoone"));
		this->gsm_modem->println(F("Content-Type: application/json"));
		this->gsm_modem->print(F("Content-Length: "));
		this->gsm_modem->println(json->measureLength());
		this->gsm_modem->println();
		json->printTo(*this->gsm_modem);
		this->gsm_modem->write(byte(26));
	}
}

bool SIM900::command(const __FlashStringHelper *command) {
	return this->command(command, "OK");
}

bool SIM900::command(const __FlashStringHelper *command, int wait){
	return this->command(command, "OK", wait);
}

bool SIM900::command(const __FlashStringHelper *command, const char* expect) {
	return this->command(command, expect, 200);
}

bool SIM900::command(const __FlashStringHelper* command, const char* expect, int wait) {
	Serial.print(F("command:"));
	Serial.println(command);
	this->gsm_modem->flush();
	this->gsm_modem->println(command);
	return this->response(expect, wait);
}

bool SIM900::response(const char* expect, int wait) {
	delay(wait);
	char buf[20] = "";
	this->readResponseToBuff(buf, 20);
	if (strncmp(buf, expect, 20) == 0) {
		return true;
	} else {
		Serial.print(F("wanted: "));
		Serial.println(expect);
		Serial.print(F("got: "));
		Serial.println(buf);
		return false;
	}
}

void SIM900::printResponse(const char* prefix) {
  Serial.print(prefix);
  delay(100);
  while(this->gsm_modem->available()!=0) {
      Serial.write((char)this->gsm_modem->read());
  }
}

void SIM900::readResponseToBuff(char * buf, const int buf_size) {
	/*
	 * GSM MODEM RESPONSE
	 * 1) \r\n
	 * 2) RESPONSE
	 * 3) \r\n
	 */
	unsigned char index = 0;
	bool got_line_feed_once = false;
	while (this->gsm_modem->available() != 0 && index < buf_size - 1) {
		char received_char = this->gsm_modem->read();
		if (received_char == '\n') {
			if (got_line_feed_once) {
				break;
			}
			got_line_feed_once = true;
		} else if (received_char == '\r') {

		} else {
			buf[index] = received_char;
			index++;
		}
	}
	buf[index] = '\0';
}
