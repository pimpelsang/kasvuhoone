/*
 * Parameters.cpp
 *
 *  Created on: Apr 1, 2018
 *      Author: erki
 */

#include "Parameter.h"
#include <EEPROM.h>
#include <stdio.h>

Parameter::Parameter(const char* name, int address, int default_value, int min_value, int max_value, bool first_boot) {
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

char * Parameter::getParameterName() {
	return this->name;
}

bool Parameter::setParameterValue(int value) {
	if (this->max_value >= value && this->min_value <= value) {
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

