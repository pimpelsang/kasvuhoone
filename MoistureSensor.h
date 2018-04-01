/*
 * MoistureSensor.h
 *
 *  Created on: Mar 27, 2018
 *      Author: erkis
 */

#ifndef MOISTURESENSOR_H_
#define MOISTURESENSOR_H_

#include "Sensor.h"

class MoistureSensor: public Sensor {

private:
	const int maxValue = 1023;
	const int minValue = 280;
	const String unit = "%";
public:
	MoistureSensor(int analogPin, String name);
	String getStringValue();
	int getValue();
};

#endif /* MOISTURESENSOR_H_ */
