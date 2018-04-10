/*
 * Parameters.cpp
 *
 *  Created on: Apr 1, 2018
 *      Author: erki
 */

#include "Parameter.h"
#include <EEPROM.h>

Parameter::Parameter(String name, int address, int default_value, int min_value, int max_value, bool first_boot) {
	this->name = name;
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

String Parameter::getParameterString() {
	return this->name + ":" + String(this->value);
}

bool Parameter::setParameterValue(int value) {
	if (this->max_value >= value && this->min_value <= value) {
		Serial.println("SET PAR VALUE!");
		int current_val;
		EEPROM.get(this->address, current_val);
		if (current_val != value) {
			EEPROM.put(this->address, value);
		}
		this->value = value;
		return true;
	} else {
		return false;
	}
}

