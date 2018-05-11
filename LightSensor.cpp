/*
 * LightSensor.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#include "LightSensor.h"
#include <stdio.h>

LightSensor::LightSensor(int analogPin): Sensor(analogPin) {
}

int LightSensor::getValue() {
	return float(Sensor::getRawValue()) / this->maxValue * 100;
}
