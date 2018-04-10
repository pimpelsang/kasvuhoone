/*
 * Parameters.h
 *
 *  Created on: Apr 1, 2018
 *      Author: erki
 */

#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <Arduino.h>

class Parameter {
private:
	String name;
	int address;
	int default_value;
	int value;
	int min_value;
	int max_value;
public:
	Parameter(String name, int address, int default_value, int min_value, int max_value, bool first_boot);
	int getParameterValue();
	bool setParameterValue(int value);
	String getParameterString();
};

#endif /* PARAMETER_H_ */
