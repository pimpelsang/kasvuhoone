#include <Arduino.h>

#include "LightSensor.h"
#include "MoistureSensor.h"
#include "Parameter.h"
#include "Relay.h"
#include "TempSensor.h"
#include <EEPROM.h>

#define LED_PIN (13)
#define EEPROM_INITIALIZED_VALUE 13

volatile bool EVERY_SEC = false;
volatile bool EVERY_MIN = false;
volatile unsigned int TIMER_SEC = 0;

String Jsonify(String name, String data){
	return "\"" + name + "\":\"" + data + "\"";
}

void setup() {
	  Serial.begin(9600);
	  Serial.println("Initializing...");
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

	  cli();//stop interrupts

	  //set timer1 interrupt at 1Hz
	  TCCR1A = 0;// set entire TCCR1A register to 0
	  TCCR1B = 0;// same for TCCR1B
	  TCNT1  = 0;//initialize counter value to 0
	  // set compare match register for 1hz increments
	  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
	  // turn on CTC mode
	  TCCR1B |= (1 << WGM12);
	  // Set CS12 and CS10 bits for 1024 prescaler
	  TCCR1B |= (1 << CS12) | (1 << CS10);
	  // enable timer compare interrupt
	  TIMSK1 |= (1 << OCIE1A);

	  Serial.println("Initialization complete.");
	  delay(100); //Allow for serial print to complete.

	  sei();//allow interrupts

	  while (true) {
		  if (EVERY_SEC == true) //Once every milliseconds
		  {
			  EVERY_SEC = false;
			  /* Toggle the LED */
			  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
		  }

		  if (EVERY_MIN == true) {
			  EVERY_MIN = false;


			  if (temp.getValue() < target_temperature.getParameterValue()) {
				  relee.activate();
			  } else {
				  relee.deactivate();
			  }

			  Serial.println("{"
					  + Jsonify(temp.getName(), temp.getStringValue()) + ","
					  + Jsonify(moist.getName(), moist.getStringValue()) + ","
					  + Jsonify(lightsens.getName(), lightsens.getStringValue())
					  + "}");
		  }
	  }
}

ISR(TIMER1_COMPA_vect){//timer0 interrupt 2kHz toggles pin 8
//generates pulse wave of frequency 2kHz/2 = 1kHz (takes two cycles for full wave- toggle high then toggle low)
	EVERY_SEC = true;
	TIMER_SEC++;
	if (TIMER_SEC %60 == 0) {
		EVERY_MIN = true;
	}
}

