/*
 * Battery.h
 *
 *  Created on: Apr 8, 2018
 *      Author: erki
 */

#ifndef BATTERY_H_
#define BATTERY_H_

#include "Sensor.h"

class Battery: public Sensor {
private:
	const String unit = "V";
	const unsigned char maxVoltage = 15;
	const int maxValue = 1023;
public:
	Battery(int analogPin, String name);
	float getValue();
	String getStringValue();
};


#endif /* BATTERY_H_ */
