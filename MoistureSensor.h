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
public:
	MoistureSensor(int analogPin, const char* name);
	void getStringValue(char* buf, int buf_size);
	int getValue();
};

#endif /* MOISTURESENSOR_H_ */
