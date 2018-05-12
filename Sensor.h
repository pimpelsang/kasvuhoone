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
	unsigned char analogPin;

public:
	Sensor(unsigned char analogPin);
	virtual ~Sensor();
	int getRawValue();
};

#endif /* SENSOR_H_ */
