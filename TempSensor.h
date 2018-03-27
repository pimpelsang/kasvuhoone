/*
 * TempSensor.h
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#ifndef TEMPSENSOR_H_
#define TEMPSENSOR_H_

#include "Sensor.h"
#include "Arduino.h"

class TempSensor: public Sensor {

public:
	TempSensor(int analogPin, String name);
	virtual ~TempSensor();
	int getValue();
};

#endif /* TEMPSENSOR_H_ */
