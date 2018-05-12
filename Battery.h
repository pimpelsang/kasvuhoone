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
public:
	Battery(unsigned char analogPin);
	float getValue();
};


#endif /* BATTERY_H_ */
