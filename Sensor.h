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
	int analogPin;

public:
	Sensor(int analogPin);
	virtual ~Sensor();
	int getRawValue();
};

#endif /* SENSOR_H_ */
