/*
 * Sensor.h
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include <Arduino.h>

class Sensor {

private:
	String name;
	int analogPin;

public:
	Sensor(int analogPin, String name);
	virtual ~Sensor();
	virtual int getValue();
	String getName();
};

#endif /* SENSOR_H_ */
