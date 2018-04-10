#include <Arduino.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

#include "LightSensor.h"
#include "MoistureSensor.h"
#include "Parameter.h"
#include "Relay.h"
#include "TempSensor.h"
#include "Events.h"
#include "Event.h"
#include "SIM900.h"
#include "Battery.h"

#define HEARTBEAT_LED_PIN (13)
#define EEPROM_INITIALIZED_VALUE 13

volatile bool EVERY_SEC = false;
volatile bool EVERY_MIN = false;
volatile unsigned int TIMER_SEC = 0;

unsigned char relay_on_counter = 0;
unsigned char relay_off_counter = 0;
#define RELAY_TRIGGER_COUNT 5

String Jsonify(String name, String data){
	return "\"" + name + "\":\"" + data + "\"";
}

void initializeTimer1() {
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

	  sei();//allow interrupts
}

void setup() {
	  Serial.begin(9600);
	  Serial.println("Initializing...");
	  delay(100); //Allow for serial print to complete.

	  // CHECK LAST EEPROM ADDRESS, TO CHECK IF FIRST BOOT OR NOT
	  bool first_boot = true;
	  if (EEPROM.read(EEPROM.length() - 1) == EEPROM_INITIALIZED_VALUE) {
		  first_boot = false;
	  } else {
		  // first boot, write initialization value to EEPROM so next boot first_boot will be false
		  EEPROM.write(EEPROM.length() - 1, EEPROM_INITIALIZED_VALUE);
	  }

	  // initialize SIM900
	  SoftwareSerial serial(7, 8);
	  SIM900 gsm_shield(&serial);

	  if (gsm_shield.initialize() == false) {
		  Serial.println("SIM900 could not be initialized!");
	  } else {
		  Serial.println("SIM900 board is ready");
		  if (gsm_shield.connectToGPRS() == true) {
			  // successful connection to GPRS
			  gsm_shield.printIP();
		  } else {
			  Serial.println("Could not connect to GPRS network!");
		  }
	  }

	  // Create Event Manager
	  Events event_manager(first_boot);

	  // register bootup event
	  // get current time from SIM900
	  Event* bootup_event = new Event(EVENT_BOOTUP, gsm_shield.getCurrentTime());
	  event_manager.writeNewEvent(*bootup_event);
	  delete bootup_event;

	  Serial.println(event_manager.getEventsString());

	  const int parameters_count = 2;

	  // INITIALIZE PARAMETERS
	  Parameter relay_on_moisture_percent("on_moisture", 0 * sizeof(int), 10, 0, 100, first_boot);
	  Parameter relay_off_moisture_percent("off_moisture", 1 * sizeof(int), 30, 0, 100, first_boot);
	  Parameter parameters_list[parameters_count] = {relay_on_moisture_percent, relay_off_moisture_percent};

	  Serial.println("Parameters list: ");
	  for (unsigned char i=0; i < parameters_count; i++) {
		  Serial.println(parameters_list[i].getParameterString());
	  }
	  // INITIALIZE SENSORS
	  TempSensor temp(0, "Temperature");
	  MoistureSensor moist(1, "Moisture");
	  LightSensor lightsens(2, "Light");
	  Relay relee(5, true);
	  Battery battery(3, "Battery");

	  pinMode(HEARTBEAT_LED_PIN,OUTPUT);

	  initializeTimer1();

	  Serial.println("Initialization complete.");
	  delay(100); //Allow for serial print to complete.

	  while (true) {
		  if (EVERY_SEC == true) //Once every milliseconds
		  {
			  EVERY_SEC = false;
			  /* Toggle the LED */
			  digitalWrite(HEARTBEAT_LED_PIN, !digitalRead(HEARTBEAT_LED_PIN));
		  }

		  if (EVERY_MIN == true) {
			  EVERY_MIN = false;

			  if (gsm_shield.gprs_connected == true) {
				  Serial.println("Try connection to Server!");
				  if (gsm_shield.connectToServer() == true){
					  gsm_shield.writeToServer("{"
							  + Jsonify(temp.getName(), temp.getStringValue()) + ","
							  + Jsonify(moist.getName(), moist.getStringValue()) + ","
							  + Jsonify(lightsens.getName(), lightsens.getStringValue()) + ","
							  + Jsonify(battery.getName(), battery.getStringValue())
							  + "}");
					  gsm_shield.disconnectFromServer();
				  }
			  } else {
				  Serial.println("GSM SHIELD NOT INITIALIZED!");
			  }

			  if (moist.getValue() < relay_on_moisture_percent.getParameterValue()) {
				  relay_on_counter++;
				  relay_off_counter = 0;
				  if (relay_on_counter >= RELAY_TRIGGER_COUNT) {
					  relay_on_counter = 0;
					  if (relee.activate()) {
						  // register relay ON event
						  // get current time from SIM900
						  Serial.println("RELAY ACTIVATED");
						  Event relay_on_event(EVENT_RELAY_ON, gsm_shield.getCurrentTime());
						  event_manager.writeNewEvent(relay_on_event);
					  }
				  }
			  } else if (moist.getValue() >= relay_off_moisture_percent.getParameterValue()){
				  relay_on_counter = 0;
				  relay_off_counter++;
				  if (relay_off_counter >= RELAY_TRIGGER_COUNT) {
					  relay_off_counter = 0;

					  if (relee.deactivate()) {
						  // register relay OFF event
						  // get current time from SIM900
						  Serial.println("RELAY DEACTIVATED");
						  Event relay_off_event(EVENT_RELAY_OFF, gsm_shield.getCurrentTime());
						  event_manager.writeNewEvent(relay_off_event);
					  }
				  }
			  }
		  }
	  }
}

ISR(TIMER1_COMPA_vect){// timer1 interrupt 1Hz
	EVERY_SEC = true;
	TIMER_SEC++;
	if (TIMER_SEC %60 == 0) {
	//if (TIMER_SEC %10 == 0) {
		EVERY_MIN = true;
	}
}

