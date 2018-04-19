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
	const unsigned char maxVoltage = 15;
	const int maxValue = 1023;
public:
	Battery(int analogPin, const char* name);
	float getValue();
	void getStringValue(char* buf, int buf_size);
};


#endif /* BATTERY_H_ */
