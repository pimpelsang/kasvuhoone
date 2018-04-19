/*
 * Battery.cpp
 *
 *  Created on: Apr 8, 2018
 *      Author: erki
 */

#include "Battery.h"
#include <stdio.h>
#include <math.h>

Battery::Battery(int analogPin, const char* name): Sensor(analogPin, name) {
}

float Battery::getValue() {
	return float(Sensor::getRawValue()) / this->maxValue * this->maxVoltage;

}

void Battery::getStringValue(char* buf, int buf_size) {
	float adc_read = this->getValue();
	char *tmpSign = (adc_read < 0) ? "-" : "";
	float tmpVal = (adc_read < 0) ? -adc_read : adc_read;

	int tmpInt1 = tmpVal;                  // Get the integer (678).
	float tmpFrac = tmpVal - tmpInt1;      // Get fraction (0.0123).
	int tmpInt2 = trunc(tmpFrac * 100);  // Turn into integer (123).

	// Print as parts, note that you need 0-padding for fractional bit.
	// snprintf(buf, buf_size, "%s%d.%02d%s", tmpSign, tmpInt1, tmpInt2, this->unit);
	snprintf(buf, buf_size, "%s%d.%02d", tmpSign, tmpInt1, tmpInt2);
}
