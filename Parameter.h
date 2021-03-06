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
	int address;
	int default_value;
	int value;
	int min_value;
	int max_value;
	bool par_changed = false;
public:
	Parameter(int address, int default_value, int min_value, int max_value, bool first_boot);
	int getParameterValue();
	bool setParameterValue(int value);
	void checkParameterValueToEEPROM();
	bool getParameterChanged();
};

#endif /* PARAMETER_H_ */
