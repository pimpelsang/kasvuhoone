#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include <WString.h>

#include "TempSensor.h"
#include "MoistureSensor.h"
#include "LightSensor.h"
#include "Relay.h"

#define LED_PIN (13)
#define POT_PIN (0)

int val = 0;
SoftwareSerial gprs(7, 8);
TempSensor temp(0, "Temperature");
MoistureSensor moist(1, "Moisture");
LightSensor lightsens(2, "Light");
Relay relee(5, true);

void setup() {
	  Serial.begin(9600);
	  Serial.println("Initialising...");
	  delay(100); //Allow for serial print to complete.

	  pinMode(LED_PIN,OUTPUT);
	  gprs.begin(19200);
	  Serial.println("Initialisation complete.");
	  delay(100); //Allow for serial print to complete.
}

void send_command(String cmd) {
	Serial.println("SEND CMD: " + cmd);
	gprs.println(cmd);
	delay(200);
}

boolean check_result(String expect) {
	String got = "";
	while(gprs.available()!=0) {
		got += (char) gprs.read();
	}
	if (got.equals("")) {
		Serial.println("GOT EMPTY RESPONSE");
		return false;
	}

	if (got.indexOf(expect) == -1) {
		Serial.println("Got wrong result: " + got);
		return false;
	}

	return true;
}

String get_result(){
	String got = "";
	while(gprs.available()!=0) {
		got += (char) gprs.read();
	}
	return got;
}


void loop() {
	  /* Toggle the LED */
	  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
	  Serial.println(temp.getName());
	  Serial.println(temp.getValue());
	  if (temp.getValue() < 600) {
		  Serial.println("ACTIVATE RELAY!");
		  relee.activate();
	  } else {
		  Serial.println("DEACTIVATE RELAY!");
		  relee.deactivate();
	  }
	  Serial.println(moist.getName());
	  Serial.println(moist.getValue());
	  Serial.println(moist.getStringValue());
	  Serial.println(lightsens.getName());
	  Serial.println(lightsens.getValue());
	  Serial.println(lightsens.getStringValue());
	  delay(1000);
}



