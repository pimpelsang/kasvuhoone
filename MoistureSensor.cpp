/*
 * MoistureSensor.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#include "MoistureSensor.h"
#include <stdio.h>

#define MAX_VALUE 1023
#define MIN_VALUE 280

MoistureSensor::MoistureSensor(unsigned char analogPin): Sensor(analogPin) {
}

int MoistureSensor::getValue() {
	return 100 - ((float(Sensor::getRawValue()) - MIN_VALUE)/(MAX_VALUE - MIN_VALUE) * 100);
}
