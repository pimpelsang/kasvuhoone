/*
 * Battery.cpp
 *
 *  Created on: Apr 8, 2018
 *      Author: erki
 */

#include "Battery.h"

Battery::Battery(int analogPin, String name): Sensor(analogPin, name) {
}

float Battery::getValue() {
	return float(Sensor::getRawValue()) / this->maxValue * this->maxVoltage;
}

String Battery::getStringValue() {
	return String(this->getValue()) + this->unit;
}
