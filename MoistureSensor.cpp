/*
 * MoistureSensor.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#include "MoistureSensor.h"

MoistureSensor::MoistureSensor(int analogPin, String name): Sensor(analogPin, name) {
	// TODO Auto-generated constructor stub

}

MoistureSensor::~MoistureSensor() {
	// TODO Auto-generated destructor stub
}

MoistureSensor::getValue() {
	return Sensor::getValue();
}
