/*
 * Battery.cpp
 *
 *  Created on: Apr 8, 2018
 *      Author: erki
 */

#include "Battery.h"
#include <stdio.h>
#include <math.h>

Battery::Battery(int analogPin): Sensor(analogPin) {
}

float Battery::getValue() {
	return float(Sensor::getRawValue()) / this->maxValue * this->maxVoltage;
}
