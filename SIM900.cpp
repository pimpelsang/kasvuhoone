/*
 * SIM900.cpp
 *
 *  Created on: Apr 4, 2018
 *      Author: erki
 */

#include "SIM900.h"
#include <Arduino.h>

SIM900::SIM900(SoftwareSerial * serial) { //configure software serial port
	this->gsm_modem = serial;
	this->gsm_modem->begin(19200);  // for GSM shield
	this->keepGSM();
}

void powerUp()
{
 pinMode(9, OUTPUT);
 digitalWrite(9,LOW);
 delay(1000);
 digitalWrite(9,HIGH);
 delay(2000);
 digitalWrite(9,LOW);
 delay(3000);
}

void SIM900::keepGSM()
{
	// test if GSM shield is active, if not call powerup
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
	if (char_count==0) powerUp(); // SIM900 did not respond
}

// converts two characters from date string into a two digit decimal value
int dateToDecimal(char * date){
	int value = ((date[0] - '0') * 10) +  (date[1] - '0');
	return value;
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

time_t SIM900::getCurrentTime() {

	String stringTime = this->getTimeString();

	int year = dateToDecimal(&stringTime[0]);
	int month = dateToDecimal(&stringTime[3]) ;
	int day = dateToDecimal(&stringTime[6]) ;
	int hour = dateToDecimal(&stringTime[9]) ;
	int minute = dateToDecimal(&stringTime[12]) ;
	int second = dateToDecimal(&stringTime[15]) ;
	setTime(hour,minute,second,day,month,year); // Another way to set
	return now();
}

