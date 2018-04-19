/*
 * MoistureSensor.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#include "MoistureSensor.h"
#include <stdio.h>

MoistureSensor::MoistureSensor(int analogPin, const char* name): Sensor(analogPin, name) {
}

int MoistureSensor::getValue() {
	return 100 - ((float(Sensor::getRawValue()) - this->minValue)/(this->maxValue - this->minValue) * 100);
}

void MoistureSensor::getStringValue(char* buf, int buf_size) {
	// snprintf(buf, buf_size, "%d%s", this->getValue(), this->unit);
	snprintf(buf, buf_size, "%d", this->getValue());
}
