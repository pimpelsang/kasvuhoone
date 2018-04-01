#include <Arduino.h>

#include "LightSensor.h"
#include "MoistureSensor.h"
#include "Parameter.h"
#include "Relay.h"
#include "TempSensor.h"
#include <EEPROM.h>

#define LED_PIN (13)
#define EEPROM_INITIALIZED_VALUE 13


String Jsonify(String name, String data){
	return "\"" + name + "\":\"" + data + "\"";
}

void setup() {
	  Serial.begin(9600);
	  Serial.println("Initialising...");
	  delay(100); //Allow for serial print to complete.

	  // CHECK LAST EEPROM ADDRESS, TO CHECK IF FIRST BOOT OR NOT
	  Serial.println("CHECKING EEPROM LAST ADDRESS VALUE");

	  bool first_boot = true;
	  if (EEPROM.read(EEPROM.length() - 1) == EEPROM_INITIALIZED_VALUE) {
		  first_boot = false;
		  Serial.println("NOT FIRST BOOT");
	  } else {
		  Serial.println("FIRST BOOT, SET DEFAULT PARAMETER VALUES");
		  EEPROM.write(EEPROM.length() - 1, EEPROM_INITIALIZED_VALUE);
	  }

	  // INITIALIZE PARAMETERS
	  Parameter target_temperature("target_temp", 0, 600, 0, 1023, first_boot);

	  // INITIALIZE SENSORS
	  TempSensor temp(0, "Temperature");
	  MoistureSensor moist(1, "Moisture");
	  LightSensor lightsens(2, "Light");
	  Relay relee(5, true);

	  pinMode(LED_PIN,OUTPUT);
	  Serial.println("Initialisation complete.");
	  delay(100); //Allow for serial print to complete.


	  while (true) {
		  /* Toggle the LED */
		  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
		  if (temp.getValue() < target_temperature.getParameterValue()) {
			  relee.activate();
		  } else {
			  relee.deactivate();
		  }
		  /*
		  Serial.println("{"
		  	  			+ Jsonify(temp.getName(), temp.getStringValue()) + ","
		  	  			+ Jsonify(moist.getName(), moist.getStringValue()) + ","
		  	  			+ Jsonify(lightsens.getName(), lightsens.getStringValue())
		  	  			+ "}");
		  */
		  delay(1000);
	  }
}




