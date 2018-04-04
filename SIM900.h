/*
 * SIM900.h
 *
 *  Created on: Apr 4, 2018
 *      Author: erki
 */

#include <Time.h>
#include <SoftwareSerial.h>

#ifndef SIM900_H_
#define SIM900_H_

class SIM900 {
private:
	SoftwareSerial * gsm_modem;
	void keepGSM();
	String getTimeString();
public:
	SIM900(SoftwareSerial * serial);
	time_t getCurrentTime();
};

#endif /* SIM900_H_ */
