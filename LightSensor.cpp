/*
 * LightSensor.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#include "LightSensor.h"

LightSensor::LightSensor(int analogPin, String name): Sensor(analogPin, name) {
}

int LightSensor::getValue() {
	return float(Sensor::getValue()) / this->maxValue * 100;
}

String LightSensor::getStringValue() {
	return String(this->getValue()) + this->unit;
}
