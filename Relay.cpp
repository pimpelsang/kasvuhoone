/*
 * Relay.cpp
 *
 *  Created on: Mar 28, 2018
 *      Author: erki
 */

#include "Relay.h"
#include <Arduino.h>

Relay::Relay(int digitalPin, bool invertedLogic) {
	// TODO Auto-generated constructor stub
	this->digitalPin = digitalPin;
	this->invertedLogic = invertedLogic;
	pinMode(digitalPin, OUTPUT);
	this->deactivate();
}

Relay::~Relay() {
	// TODO Auto-generated destructor stub
}

void Relay::activate() {
	if (this->state == true) {
		Serial.println("RELAY ALREADY ACTIVATED!");
		return;
	}
	if (this->invertedLogic) {
		digitalWrite(this->digitalPin, LOW);
	} else {
		digitalWrite(this->digitalPin, HIGH);
	}
	this->state = true;
}

void Relay::deactivate() {
	if (this->state == false) {
		Serial.println("RELAY ALREADY DEACTIVATED!");
		return;
	}
	if (this->invertedLogic) {
		digitalWrite(this->digitalPin, HIGH);
	} else {
		digitalWrite(this->digitalPin, LOW);
	}
	this->state = false;
}

bool Relay::getState() {
	return this->state;
}
