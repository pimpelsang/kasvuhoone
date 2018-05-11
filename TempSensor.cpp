/*
 * TempSensor.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: erki
 */

#include "TempSensor.h"
#include <stdio.h>
#include <Arduino.h>
#include <math.h>

#define SERIES_RESISTOR 1000.0
#define BETA 3470.0
#define ADC_MAX 1023
#define THERMISTOR_RESISTANCE_ROOM_TEMP 1000
#define ROOM_TEMP_KELVIN 298.15

TempSensor::TempSensor(int analogPin): Sensor(analogPin) {
}
/*
(Ground) ----\/\/\/-------|-------\/\/\/---- V_supply
           R_balance      |     R_thermistor
                          |
                     Analog Pin
*/

int TempSensor::getValue() {
	// variables that live in this function
	double rThermistor;            // Holds thermistor resistance value
	double tKelvin;            // Holds calculated temperature
	int tCelsius;            // Hold temperature in celsius
	double adcAverage  = 0;            // Holds the average voltage measurement
	int    adcSamples[5];  // Array to hold each voltage measurement

	for (int i = 0; i < 5; i++)
	{
	adcSamples[i] = Sensor::getRawValue();;
	delay(10);
	}

	for (int i = 0; i < 5; i++)
	{
	adcAverage += adcSamples[i];      // add all samples up . . .
	}
	adcAverage /= 5;        // . . . average it w/ divide


	rThermistor = SERIES_RESISTOR * ( (1023 / adcAverage) - 1);

	tKelvin = (BETA * ROOM_TEMP_KELVIN) /
			(BETA + (ROOM_TEMP_KELVIN * log(rThermistor / THERMISTOR_RESISTANCE_ROOM_TEMP)));

	tCelsius = tKelvin - 273.15;  // convert kelvin to celsius

	return tCelsius;    // Return the temperature in Celsius
}
