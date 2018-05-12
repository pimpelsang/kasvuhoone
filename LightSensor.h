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
public:
	LightSensor(unsigned char analogPin);
	int getValue();
};

#endif /* LIGHTSENSOR_H_ */
