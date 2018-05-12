/*
 * Parameters.cpp
 *
 *  Created on: Apr 1, 2018
 *      Author: erki
 */

#include "Parameter.h"
#include <EEPROM.h>
#include <stdio.h>
#include <Arduino.h>


Parameter::Parameter(int address, int default_value, int min_value, int max_value, bool first_boot) {
	this->address = address;
	this->default_value = default_value;
	this->min_value = min_value;
	this->max_value = max_value;

	if (first_boot) {
		this->setParameterValue(this->default_value);
	} else {
		EEPROM.get(this->address, this->value);
	}
}

int Parameter::getParameterValue() {
	return this->value;
}

void Parameter::checkParameterValueToEEPROM() {
	if (this->par_changed){
		this->par_changed = false;
		Serial.print("PAR -> EEPROM: ");
		Serial.println(this->address);
		EEPROM.update(this->address, this->value);
	}
}

bool Parameter::setParameterValue(int value) {
	if (this->max_value >= value && this->min_value <= value) {
		this->par_changed = true;
		this->value = value;
		return true;
	} else {
		return false;
	}
}

bool Parameter::getParameterChanged() {
	return this->par_changed;
}

