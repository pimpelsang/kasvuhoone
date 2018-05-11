/*
 * LightSensor.h
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#ifndef LIGHTSENSOR_H_
#define LIGHTSENSOR_H_

#include "Sensor.h"

class LightSensor: public Sensor {
private:
	const int maxValue = 1023;
public:
	LightSensor(int analogPin);
	int getValue();
};

#endif /* LIGHTSENSOR_H_ */
