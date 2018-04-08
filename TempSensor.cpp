/*
 * TempSensor.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#include "TempSensor.h"

TempSensor::TempSensor(int analogPin, String name): Sensor(analogPin, name) {
}

int TempSensor::getValue() {
	return Sensor::getRawValue(); // calls base class' functio
}

String TempSensor::getStringValue() {
	return String(this->getValue()) + this->unit;
}
