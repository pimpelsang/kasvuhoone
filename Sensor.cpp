/*
 * Sensor.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#include "Sensor.h"
#include <Arduino.h>

Sensor::Sensor(int analogPin, const char* name) {
	this->analogPin = analogPin;
	this->name = name;
}

Sensor::~Sensor() {
}

int Sensor::getRawValue() {
	return analogRead(this->analogPin);
}

char* Sensor::getName() {
	return this->name;
}
