/*
 * Sensor.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#include "Sensor.h"
#include <Arduino.h>

Sensor::Sensor(int analogPin, String name) {
	this->analogPin = analogPin;
	this->name = name;
}

Sensor::~Sensor() {
}

int Sensor::getValue() {
	return analogRead(this->analogPin);
}

String Sensor::getName() {
	return this->name;
}
