/*
 * TempSensor.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#include "TempSensor.h"

TempSensor::TempSensor(int analogPin, String name): Sensor(analogPin, name) {

	// TODO Auto-generated constructor stub

}

TempSensor::~TempSensor() {
	// TODO Auto-generated destructor stub
}

int TempSensor::getValue() {
	return Sensor::getValue(); // calls base class' functio
}
