/*
 * TempSensor.h
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#ifndef TEMPSENSOR_H_
#define TEMPSENSOR_H_

#include "Sensor.h"

class TempSensor: public Sensor {
private:
public:
	TempSensor(int analogPin);
	int getValue();
};

#endif /* TEMPSENSOR_H_ */
