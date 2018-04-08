/*
 * MoistureSensor.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#include "MoistureSensor.h"

MoistureSensor::MoistureSensor(int analogPin, String name): Sensor(analogPin, name) {
}

int MoistureSensor::getValue() {
	return 100 - ((float(Sensor::getRawValue()) - this->minValue)/(this->maxValue - this->minValue) * 100);
}

String MoistureSensor::getStringValue() {
	return String(this->getValue()) + this->unit;
}
