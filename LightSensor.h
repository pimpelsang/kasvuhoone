/*
 * LightSensor.h
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#ifndef LIGHTSENSOR_H_
#define LIGHTSENSOR_H_

#include "Sensor.h"
#include <Arduino.h>

class LightSensor: public Sensor {
private:
	const String unit = "%";
	const int maxValue = 1023;
public:
	LightSensor(int analogPin, String name);
	virtual ~LightSensor();
	int getValue();
	String getStringValue();
};

#endif /* LIGHTSENSOR_H_ */
