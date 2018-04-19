/*
 * Parameters.h
 *
 *  Created on: Apr 1, 2018
 *      Author: erki
 */

#ifndef PARAMETER_H_
#define PARAMETER_H_

class Parameter {
private:
	char* name;
	int address;
	int default_value;
	int value;
	int min_value;
	int max_value;
public:
	Parameter(const char* name, int address, int default_value, int min_value, int max_value, bool first_boot);
	int getParameterValue();
	bool setParameterValue(int value);
	char* getParameterName();
};

#endif /* PARAMETER_H_ */
