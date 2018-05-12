/*
 * LightSensor.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#include "LightSensor.h"
#include <stdio.h>

#define MAX_VALUE 1023

LightSensor::LightSensor(unsigned char analogPin): Sensor(analogPin) {
}

int LightSensor::getValue() {
	return float(Sensor::getRawValue()) / MAX_VALUE * 100;
}
