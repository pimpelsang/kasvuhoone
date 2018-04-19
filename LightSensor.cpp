/*
 * LightSensor.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#include "LightSensor.h"
#include <stdio.h>

LightSensor::LightSensor(int analogPin, const char* name): Sensor(analogPin, name) {
}

int LightSensor::getValue() {
	return float(Sensor::getRawValue()) / this->maxValue * 100;
}

void LightSensor::getStringValue(char* buf, int buf_size) {
	// snprintf(buf, buf_size, "%d%s", this->getValue(), this->unit);
	snprintf(buf, buf_size, "%d", this->getValue());
}
