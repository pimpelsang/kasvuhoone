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
public:
	LightSensor(int analogPin, String name);
	virtual ~LightSensor();
	int getValue();
};

#endif /* LIGHTSENSOR_H_ */
