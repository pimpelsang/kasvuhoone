#define TINY_GSM_MODEM_SIM900

#include <TinyGsmClient.h>
#include <ThingerTinyGSM.h>
#include <SoftwareSerial.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <Time.h>

#include "MoistureSensor.h"
#include "Relay.h"
#include "Parameter.h"
#include "Battery.h"
#include "Events.h"
#include "Event.h"
#include "TempSensor.h"
#include "Fan.h"

#define EEPROM_INITIALIZED_VALUE 13


SoftwareSerial SerialAT(7, 8); // RX, TX

// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "internet.emt.ee";
const char user[] = "";
const char pass[] = "";

ThingerTinyGSM thing("kasvuhoone", "kasvuhoone", "$D5sAiZpkWJy", SerialAT);
TinyGsm *gsm;
MoistureSensor moist(0);
MoistureSensor moist2(2);
Relay relee(5, true);
Battery battery(3);
TempSensor temp(1);
Events *event_manager;
Fan fan(6);

Parameter *relay_on_moisture_percent;
Parameter *relay_off_moisture_percent;
Parameter *fan_duty_cycle;

volatile bool EVERY_SEC = false;
// EVERY_MIN is set to true so it will be activated on first main loop iteration
volatile bool EVERY_MIN = true;
volatile unsigned int TIMER_SEC = 0;

unsigned char relay_on_counter = 0;
unsigned char relay_off_counter = 0;
unsigned char bootup_event = true;
#define RELAY_TRIGGER_COUNT 5

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

void setup() {

	Serial.begin(115200);
	delay(10);

	// Set GSM module baud rate
	SerialAT.begin(19200);
	delay(3000);

	Serial.println(F("INIT.."));

	gsm = &thing.getTinyGsm();
	if (!gsm->testAT()){
		powerUp();
	}

	// CHECK LAST EEPROM ADDRESS, TO CHECK IF FIRST BOOT OR NOT

	bool first_boot = true;
	if (EEPROM.read(EEPROM.length() - 1) == EEPROM_INITIALIZED_VALUE) {
		first_boot = false;
	} else {
		// first boot, write initialization value to EEPROM so next boot first_boot will be false
		EEPROM.write(EEPROM.length() - 1, EEPROM_INITIALIZED_VALUE);
	}

	// Create Event Manager
	event_manager = new Events(first_boot);

	relay_on_moisture_percent = new Parameter(0 * sizeof(int), 10, 0, 100, first_boot);
	relay_off_moisture_percent = new Parameter(1 * sizeof(int), 30, 0, 100, first_boot);
	fan_duty_cycle = new Parameter(2 * sizeof(int), 25, 0, 100, first_boot);

	thing.setAPN(apn, user, pass);

	thing["relay"] << [](pson& in){
	  if(in.is_empty()){
		  in = relee.getState();
	  }
	  else{
		  in ? relee.activate() : relee.deactivate();
	  }
	};


	thing["moistBat"] >> [](pson& out){
	      out["moisture1"] = moist.getValue();
	      out["moisture2"] = moist2.getValue();
	      out["battery"] = battery.getValue();
	      out["signal"] = gsm->getSignalQuality();
	      out["temp"] = temp.getValue();
	};

	thing["event"] >> outputValue(event_manager->last_event_number);


	thing["relay_off_%"] << [](pson& in){
		if(in.is_empty()){
			in = relay_off_moisture_percent->getParameterValue();
		}
		else{
			relay_off_moisture_percent->setParameterValue(in);
		}
	};

	thing["relay_on_%"] << [](pson& in){
		if(in.is_empty()){
			in = relay_on_moisture_percent->getParameterValue();
		}
		else{
			relay_on_moisture_percent->setParameterValue(in);
		}
	};

	thing["fan_duty_cycle"] << [](pson& in){
		if(in.is_empty()){
			in = fan_duty_cycle->getParameterValue();
		}
		else{
			fan_duty_cycle->setParameterValue(in);
		}
	};

	thing["fan_boolean"] << [](pson& in){
		if(in.is_empty()){
		  in = fan.active;
		}
		else{
		  in ? fan.activate(fan_duty_cycle->getParameterValue()) : fan.deactivate();
		}
	};

	initializeTimer1();

	Serial.println(F("INIT DONE"));
}

void loop() {
	thing.handle();

	if (EVERY_SEC == true) //Once every milliseconds
	{
	  EVERY_SEC = false;

	  if (fan_duty_cycle->getParameterChanged()) {
		  fan.setDutyCycle(fan_duty_cycle->getParameterValue());
	  }

	  Serial.print("FAN SPEED: ");
	  Serial.println(fan_duty_cycle->getParameterValue());
	}

	if (EVERY_MIN == true) {
		EVERY_MIN = false;

		if (bootup_event) {
			bootup_event = false;
			Event bootup_event(EVENT_BOOTUP, 0);
			event_manager->writeNewEvent(bootup_event);
			thing.write_bucket("Events", "event");
		}


		// CHECK IF ANY PARAMETER HAS CHANGED AND NEW VALUE HAS TO BE STORED TO EEPROM
		relay_off_moisture_percent->checkParameterValueToEEPROM();
		relay_on_moisture_percent->checkParameterValueToEEPROM();
		fan_duty_cycle->checkParameterValueToEEPROM();

		if (moist.getValue() < relay_on_moisture_percent->getParameterValue()) {
			relay_on_counter++;
			relay_off_counter = 0;
			if (relay_on_counter >= RELAY_TRIGGER_COUNT) {
				relay_on_counter = 0;

				if (relee.activate()) {
					// register relay ON event
					// get current time from SIM900
					// Event relay_on_event(EVENT_RELAY_ON, gsm_shield.getCurrentTime());
					Event relay_on_event(EVENT_RELAY_ON, 0);
					event_manager->writeNewEvent(relay_on_event);
					Serial.println(event_manager->last_event_number);
					thing.write_bucket("Events", "event");
				}
			}
		} else if (moist.getValue() >= relay_off_moisture_percent->getParameterValue()){
			relay_on_counter = 0;
			relay_off_counter++;
			if (relay_off_counter >= RELAY_TRIGGER_COUNT) {
				relay_off_counter = 0;

				if (relee.deactivate()) {
					// register relay OFF event
					// get current time from SIM900
					//Event relay_off_event(EVENT_RELAY_OFF, gsm_shield.getCurrentTime());
					Event relay_off_event(EVENT_RELAY_OFF, 0);
					event_manager->writeNewEvent(relay_off_event);
					Serial.println(event_manager->last_event_number);
					thing.write_bucket("Events", "event");
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
