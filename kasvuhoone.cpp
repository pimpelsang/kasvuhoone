#include <Arduino.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

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
#define MAX_INPUT 200

volatile bool EVERY_SEC = false;
// EVERY_MIN is set to true so it will be activated on first main loop iteration
volatile bool EVERY_MIN = true;
volatile unsigned int TIMER_SEC = 0;

unsigned char relay_on_counter = 0;
unsigned char relay_off_counter = 0;
#define RELAY_TRIGGER_COUNT 5

#define PARAMETERS_COUNT 2


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


void process_data(const char * data, unsigned int buf_len)
  {
  // for now just display it
  // (but you could compare it to some value, convert to an integer, etc.)
	Serial.println (data);
  }  // end of process_data

void processIncomingByte (const byte inByte)
  {
  static char input_line [MAX_INPUT];
  static unsigned int input_pos = 0;

  switch (inByte)
    {

    case '\n':   // end of text
      input_line [input_pos] = 0;  // terminating null byte

      // terminator reached! process input_line here ...
      process_data (input_line, input_pos);

      // reset buffer for next time
      input_pos = 0;
      break;

    case '\r':   // discard carriage return
      break;

    default:
      // keep adding if not full ... allow for terminating null byte
      if (input_pos < (MAX_INPUT - 1))
        input_line [input_pos++] = inByte;
      break;

    }  // end of switch

  } // end of processIncomingByte


void printEventsToSerial(Events *event_manager) {
	StaticJsonBuffer<EVENTS_COUNT * 25> jsonBuffer;
	JsonObject& json = jsonBuffer.createObject();

	char numbuf[10];

	for (char i=0; i < EVENTS_COUNT; i++) {
		Event event = event_manager->getEventByOffset(i);
		if (event.event_number == 255) {
			// no more filled events found
			break;
		}

		snprintf(numbuf, 10, "%lu", event.event_time);

		json[numbuf] = event.event_number;
	}
	json.prettyPrintTo(Serial);
}

void printParametersToSerial(Parameter parameters_list[]) {
	StaticJsonBuffer<PARAMETERS_COUNT * 30> jsonBuffer;

	JsonObject& json = jsonBuffer.createObject();

	for (unsigned char i=0; i < PARAMETERS_COUNT; i++) {
		json[parameters_list[i].getParameterName()] = parameters_list[i].getParameterValue();
	}
	json.prettyPrintTo(Serial);
}

void setup() {
	  Serial.begin(9600);
	  Serial.println(F("Init"));
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
		  Serial.println(F("SIM900 could not be initialized!"));
	  } else {
		  Serial.println(F("SIM900 board is ready"));

		  if (gsm_shield.connectToGPRS() == true) {
			  // successful connection to GPRS
			  gsm_shield.printIP();
			  gsm_shield.disconnectFromServer();
		  }
	  }

	  // Create Event Manager
	  Events event_manager(first_boot);


	  // register bootup event
	  // get current time from SIM900
	  Event* bootup_event = new Event(EVENT_BOOTUP, gsm_shield.getCurrentTime());
	  event_manager.writeNewEvent(*bootup_event);
	  delete bootup_event;


	  printEventsToSerial(&event_manager);

	  // INITIALIZE PARAMETERS
	  Parameter relay_on_moisture_percent("on_moisture", 0 * sizeof(int), 10, 0, 100, first_boot);
	  Parameter relay_off_moisture_percent("off_moisture", 1 * sizeof(int), 30, 0, 100, first_boot);
	  Parameter parameters_list[PARAMETERS_COUNT] = {relay_on_moisture_percent, relay_off_moisture_percent};

	  printParametersToSerial(parameters_list);



	  // INITIALIZE SENSORS
	  TempSensor temp(0, "Temperature");
	  MoistureSensor moist(1, "Moisture");
	  LightSensor lightsens(2, "Light");
	  Relay relee(5, true);
	  Battery battery(3, "Battery");

	  pinMode(HEARTBEAT_LED_PIN,OUTPUT);

	  initializeTimer1();

	  Serial.println(F("Init complete."));
	  delay(100); //Allow for serial print to complete.

	  while (true) {


		  while (gsm_shield.gsm_modem->available() > 0) {
			  processIncomingByte(gsm_shield.gsm_modem->read());
		  }



		  if (EVERY_SEC == true) //Once every milliseconds
		  {
			  EVERY_SEC = false;
			  /* Toggle the LED */
			  digitalWrite(HEARTBEAT_LED_PIN, !digitalRead(HEARTBEAT_LED_PIN));
		  }

		  if (EVERY_MIN == true) {
			  EVERY_MIN = false;

			  StaticJsonBuffer<200> jsonBuffer;

			  JsonObject& json = jsonBuffer.createObject();
			  char tempbuf[10] = "";
			  temp.getStringValue(tempbuf, 10);
			  json[temp.getName()] = tempbuf;

			  char batterybuf[10] = "";
			  battery.getStringValue(batterybuf, 10);
			  json[battery.getName()] = batterybuf;

			  char moistbuf[10] = "";
			  moist.getStringValue(moistbuf, 10);
			  json[moist.getName()] = moistbuf;

			  char lightbuf[10] = "";
			  lightsens.getStringValue(lightbuf, 10);
			  json[lightsens.getName()] = lightbuf;

			  json.prettyPrintTo(Serial);

			  if (gsm_shield.gprs_connected == true) {
				  gsm_shield.server_connected = gsm_shield.connectToServer();
				  if (gsm_shield.server_connected) {
					  Serial.println(F("Server connected"));
					  gsm_shield.writeJsonToServer(&json);
				  } else {
					  Serial.println(F("Could not connect"));
				  }
			  }


			  if (moist.getValue() < relay_on_moisture_percent.getParameterValue()) {
				  relay_on_counter++;
				  relay_off_counter = 0;
				  if (relay_on_counter >= RELAY_TRIGGER_COUNT) {
					  relay_on_counter = 0;
					  if (relee.activate()) {
						  // register relay ON event
						  // get current time from SIM900
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

