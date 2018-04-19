/*
 * TempSensor.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#include "TempSensor.h"
#include <stdio.h>

TempSensor::TempSensor(int analogPin, const char* name): Sensor(analogPin, name) {
}

int TempSensor::getValue() {
	return Sensor::getRawValue(); // calls base class' functio
}

void TempSensor::getStringValue(char* buf, int buf_size) {
	// snprintf(buf, buf_size, "%d%s", this->getValue(), this->unit);
	snprintf(buf, buf_size, "%d", this->getValue());
}
