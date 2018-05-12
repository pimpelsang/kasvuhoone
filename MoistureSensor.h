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
public:
	MoistureSensor(unsigned char analogPin);
	int getValue();
};

#endif /* MOISTURESENSOR_H_ */
