/*
 * MoistureSensor.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#include "MoistureSensor.h"
#include <stdio.h>

MoistureSensor::MoistureSensor(int analogPin): Sensor(analogPin) {
}

int MoistureSensor::getValue() {
	return 100 - ((float(Sensor::getRawValue()) - this->minValue)/(this->maxValue - this->minValue) * 100);
}
