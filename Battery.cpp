/*
 * Battery.cpp
 *
 *  Created on: Apr 8, 2018
 *      Author: erki
 */

#include "Battery.h"
#include <stdio.h>
#include <math.h>

#define MAX_VOLTAGE 15
#define MAX_VALUE 1023

Battery::Battery(unsigned char analogPin): Sensor(analogPin) {
}

float Battery::getValue() {
	return float(Sensor::getRawValue()) / MAX_VALUE * MAX_VOLTAGE;
}
