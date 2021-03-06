/*
 * event.cpp
 *
 *  Created on: Apr 4, 2018
 *      Author: erki
 */

#include <Arduino.h>
#include "Event.h"
#include <string.h>

Event::Event(unsigned char event_number, time_t event_time) {
	this->event_number = event_number;
	this->event_time = event_time;
}

void Event::printEventString(){
	Serial.println("Event Number:");
	Serial.println(this->event_number);
	Serial.println("Event Time:");
	Serial.println(this->event_time);
}

void Event::getEventString(char *buf, const int buf_size) {
	snprintf(buf, buf_size, "%d%s%lu", this->event_number, ":", this->event_time);
}
