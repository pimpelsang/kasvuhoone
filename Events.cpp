/*
 * events.cpp
 *
 *  Created on: Apr 4, 2018
 *      Author: erki
 */

#include <Arduino.h>
#include <EEPROM.h>
#include "Events.h"

Events::Events(bool first_boot) {
	if (first_boot == true) {
		EEPROM.write(EVENT_OFFSET_ADDRESS, 0);
		this->last_event_eeprom_offset = 0;
		this->clearEventsLog();
	} else {
		this->last_event_eeprom_offset = EEPROM.read(EVENT_OFFSET_ADDRESS);
	}
	Serial.println("EVENT OFFSET");
	Serial.println(this->last_event_eeprom_offset);
}

void Events::clearEventsLog(){
	// write EVENT 255(unused address) to all event numbers
	for (unsigned char i=0; i<EVENTS_COUNT; i++) {
		EEPROM.update(EVENTS_START_ADDRESS + i * EVENT_SIZE_BYTES, 255);
	}
}

Event Events::getLastEvent(){
	return this->getEventByOffset(0);
}

Event Events::getEventByOffset(unsigned char offset){
	// last event has offset 0
	// max offset is 9
	if (offset > this->last_event_eeprom_offset) {
		// offset = 6, current = 4 -> WE WANT TO RETURN EVENT AT OFFSET 8 = (10 - (6 - 4))
		// offset = 5, current = 1 -> WE WANT TO RETURN EVENT AT OFFSET 6 = (10 - (5 -1))
		offset = EVENTS_COUNT - (offset - this->last_event_eeprom_offset);
	} else {
		// offset = 4, current = 7 -> WE WANT TO RETURN EVENT AT OFFSET 3 = (7 - 4)
		offset = this->last_event_eeprom_offset - offset;
	}

	unsigned char event_number = EEPROM.read(EVENTS_START_ADDRESS + offset * EVENT_SIZE_BYTES);
	time_t event_time;
	EEPROM.get(EVENTS_START_ADDRESS + offset * EVENT_SIZE_BYTES + 1, event_time);
	Event event(event_number, event_time);
	return event;
}

void Events::writeNewEvent(Event event) {
	this->last_event_eeprom_offset++;
	if (this->last_event_eeprom_offset >= EVENTS_COUNT) {
		this->last_event_eeprom_offset = 0;
	}
	EEPROM.write(EVENTS_START_ADDRESS + this->last_event_eeprom_offset * EVENT_SIZE_BYTES, event.event_number);
	EEPROM.put(EVENTS_START_ADDRESS + this->last_event_eeprom_offset * EVENT_SIZE_BYTES + 1, event.event_time);
	EEPROM.write(EVENT_OFFSET_ADDRESS, this->last_event_eeprom_offset);
}

void Events::printAllEvents(){
	for (char i=0; i < EVENTS_COUNT; i++) {
		Event event = this->getEventByOffset(i);
		if (event.event_number == 255) {
			// no more filled events found
			return;
		}
		event.printEventString();
	}
}

String Events::getEventsString(){
	String events_string = "";
	for (char i=0; i < EVENTS_COUNT; i++) {
		Event event = this->getEventByOffset(i);
		if (event.event_number == 255) {
			// no more filled events found
			break;
		}
		if (i != 0) {
			events_string += ",";
		}
		events_string += event.getEventString();
	}
	return events_string;
}
