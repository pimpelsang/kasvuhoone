/*
 * Fan.h
 *
 *  Created on: Mar 28, 2018
 *      Author: erki
 */

#ifndef FAN_H_
#define FAN_H_

#include <FanController.h>

class Fan {
private:
	FanController *fan;
public:
	Fan(int digitalPin);
	bool active = false;
	virtual ~Fan();
	void activate(int duty_cycle);
	void deactivate();
	void setDutyCycle(int duty_cycle);
};

#endif /* FAN_H_ */
