/*
 * Sensor.h
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#ifndef SENSOR_H_
#define SENSOR_H_

class Sensor {

private:
	char* name;
	int analogPin;

public:
	Sensor(int analogPin, const char* name);
	virtual ~Sensor();
	int getRawValue();
	char* getName();
};

#endif /* SENSOR_H_ */
