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
			if (this->command("ATE0") == false){
				//turn echo off
				Serial.println("Could not turn echo off!");
				continue;
			}
			Serial.println("ECHO IS TURNED OFF!");
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
	this->gsm_modem->println("AT+CPAS"); //here we check SIM900 activity
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

String SIM900::getTimeString() {
	// AT+CCLK? returns string 18/04/03,09:10:26
	String stringOne = "";
	String stringTime = "";
	char inchar;
	this->gsm_modem->println("AT+CCLK?");
	delay(100);
	while(this->gsm_modem->available()){
		inchar = this->gsm_modem->read();
		stringOne.concat(inchar);
	}

	for (int s=0; s<(stringOne.length()-6); s++) {
		if (stringOne.substring(s,s+6) == "+CCLK:")
		{
			stringTime = (stringOne.substring(s+8,s+25));
		}
	}
	return stringTime;
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

	String stringTime = this->getTimeString();

	int year = dateToDecimal(&stringTime[0]);
	int month = dateToDecimal(&stringTime[3]) ;
	int day = dateToDecimal(&stringTime[6]) ;
	int hour = dateToDecimal(&stringTime[9]) ;
	int minute = dateToDecimal(&stringTime[12]) ;
	int second = dateToDecimal(&stringTime[15]) ;
	setTime(hour,minute,second,day,month,year); // set arduino time library time so correct time_t object will be returned with now()
	return now();
}

bool SIM900::connectToGPRS(){
	// first test current connection status
	if (this->command("AT+CIFSR", "ERROR", 1000) == false){
		// got IP instead of ERROR, connect to GPRS not needed!
		this->gprs_connected = true;
		return true;
	}
	Serial.print("Connecting to GRPS Network");
	this->command("AT+CGATT=1"); // attach to GPRS Service
	delay(3000);
	this->command("AT+CIPMUX=0"); //We only want a single IP connection
	this->command("AT+CIPMODE=0"); //Selecting Normal Mode
	this->command("AT+CGDCONT=1,\"IP\",\"internet.emt.ee\"");
	delay(2000);
	this->command("AT+CSTT=\"internet.emt.ee\",\"\",\"\"", 3000); //Start Task and set Access Point Name
	delay(1000);
	if (this->command("AT+CIICR", 10000) == false) { // create wireless connection to GPRS (should get IP after this)
		this->gprs_connected = false;
		return false;
	}
	this->gprs_connected = true;
	return true;
}

void SIM900::printIP() {
	this->gsm_modem->println("AT+CIFSR");
	delay(1000);
	this->printResponse("IP:");
}

bool SIM900::connectToServer() {
	// this->command("AT+CIPSTART=\"TCP\",\"beta.pelltech.eu\",\"8236\"", 5000);
	// if (this->command("AT+CIPSTART=\"TCP\",\"erki.pelltech.eu\",\"8234\"", 5000) == true){
	if (this->command("AT+CIPSTART=\"TCP\",\"kasvuhoone-echo.herokuapp.com\",\"80\"", 5000) == true){
		this->server_connected = true;
		Serial.println("Connected to server!");
		return true;
	} else {
		this->disconnectFromServer();
		return false;
	}
}

bool SIM900::disconnectFromServer() {
	if (this->command("AT+CIPCLOSE", "CLOSE OK", 5000)) {
		this->server_connected = false;
		Serial.println("Disconnected from server!");
		return true;
	} else {
		return false;
	}
}

bool SIM900::writeToServer(String message) {
	Serial.println("Writing message to server: ");
	Serial.println(message);
	if (this->command("AT+CIPSEND", ">") == true){
		this->gsm_modem->println(message);
		delay(300);
		this->gsm_modem->write(byte(26));
		delay(2000);
		this->printResponse("SERVER RESPONSE: ");
		return true;
	}
	return false;
}

bool SIM900::command(String command) {
	return this->command(command, "OK");
}

bool SIM900::command(String command, int wait){
	return this->command(command, "OK", wait);
}

bool SIM900::command(String command, String expect) {
	return this->command(command, expect, 200);
}

bool SIM900::command(String command, String expect, int wait) {
	Serial.println("Writing command:");
	Serial.println(command);
	this->gsm_modem->println(command);
	return this->response(expect, wait);
}

bool SIM900::response(String expect, int wait) {
	delay(wait);
    String got = this->readResponse();

    //check response
    if (got.equals("")) {
      Serial.println(" response().no response!");
      return false;
    }

    if (got.indexOf(expect) == -1) {
      Serial.println(" response().no match! got: "+got);
      return false;
    }

    return true;
}

void SIM900::printResponse(String prefix) {
  Serial.print(prefix);
  delay(100);
  while(this->gsm_modem->available()!=0) {
      Serial.write((char)this->gsm_modem->read());
  }
}

String SIM900::readResponse(){
	String got = "";
	delay(100);
	while(this->gsm_modem->available()!=0) {
		got += (char) this->gsm_modem->read();
	}
	return got;
}
