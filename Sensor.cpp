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
	// TODO Auto-generated constructor stub
}

Sensor::~Sensor() {
	// TODO Auto-generated destructor stub
}

int Sensor::getValue() {
	return analogRead(this->analogPin);
}

String Sensor::getName() {
	return this->name;
}
