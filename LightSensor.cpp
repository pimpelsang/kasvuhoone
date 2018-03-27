/*
 * LightSensor.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#include "LightSensor.h"

LightSensor::LightSensor(int analogPin, String name): Sensor(analogPin, name) {
	// TODO Auto-generated constructor stub

}

LightSensor::~LightSensor() {
	// TODO Auto-generated destructor stub
}

int LightSensor::getValue() {
	return Sensor::getValue(); // calls base class' functio
}

