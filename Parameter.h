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
	bool write_to_eeprom = false;
public:
	Parameter(int address, int default_value, int min_value, int max_value, bool first_boot);
	int getParameterValue();
	bool setParameterValue(int value);
	void checkParameterValueToEEPROM();
};

#endif /* PARAMETER_H_ */
