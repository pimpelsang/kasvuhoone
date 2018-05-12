/*
 * Sensor.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#include "Sensor.h"
#include <Arduino.h>

Sensor::Sensor(unsigned char analogPin) {
	this->analogPin = analogPin;
}

Sensor::~Sensor() {
}

int Sensor::getRawValue() {
	return analogRead(this->analogPin);
}
