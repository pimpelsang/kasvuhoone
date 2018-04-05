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
	bool testGSM();
	String getTimeString();
	bool response(String expect, int wait);
	bool command(String command);
	bool command(String command, String expect);
	bool command(String command, int wait);
	bool command(String command, String expect, int wait);
	void printResponse(String prefix);
	String readResponse();
public:
	bool gprs_connected = false;
	bool server_connected = false;
	bool initialized = false;
	SIM900(SoftwareSerial * serial);
	time_t getCurrentTime();
	bool initialize();
	bool connectToGPRS();
	bool connectToServer();
	bool writeToServer(String message);
	void printIP();
};

#endif /* SIM900_H_ */
