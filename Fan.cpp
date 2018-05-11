/*
 * Fan.cpp
 *
 *  Created on: Mar 28, 2018
 *      Author: erki
 */

#include "Fan.h"

// Choose a threshold in milliseconds between readings.
// A smaller value will give more updated results,
// while a higher value will give more accurate and smooth readings
#define SENSOR_THRESHOLD 1000

// Sensor wire is plugged into port 2 on the Arduino.
// For a list of available pins on your board,
// please refer to: https://www.arduino.cc/en/Reference/AttachInterrupt
#define SENSOR_PIN 4

Fan::Fan(int pwmPin) {
	// Initialize library
	this->fan = new FanController(SENSOR_PIN, SENSOR_THRESHOLD, pwmPin);

	this->fan->begin();

	this->deactivate();
}

Fan::~Fan() {
	// TODO Auto-generated destructor stub
}

void Fan::activate(int duty_cycle) {
	this->active = true;
	this->fan->setDutyCycle(byte(duty_cycle));
}

void Fan::setDutyCycle(int duty_cycle) {
	if (this->active) {
		this->fan->setDutyCycle(byte(duty_cycle));
	}
}

void Fan::deactivate() {
	this->active = false;
	this->fan->setDutyCycle(0);

}

