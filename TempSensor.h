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
	const String unit = "°C";
public:
	TempSensor(int analogPin, String name);
	int getValue();
	String getStringValue();
};

#endif /* TEMPSENSOR_H_ */
