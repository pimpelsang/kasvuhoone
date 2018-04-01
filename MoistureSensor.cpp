/*
 * MoistureSensor.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#include "MoistureSensor.h"
#include "Arduino.h"

MoistureSensor::MoistureSensor(int analogPin, String name): Sensor(analogPin, name) {
	// TODO Auto-generated constructor stub

}

MoistureSensor::~MoistureSensor() {
	// TODO Auto-generated destructor stub
}

int MoistureSensor::getValue() {
	return 100 - ((float(Sensor::getValue()) - this->minValue)/(this->maxValue - this->minValue) * 100);
}

String MoistureSensor::getStringValue() {
	return String(this->getValue()) + this->unit;
}
