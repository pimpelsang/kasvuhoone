/*
 * SIM900.h
 *
 *  Created on: Apr 4, 2018
 *      Author: erki
 */

#include <Time.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#ifndef SIM900_H_
#define SIM900_H_

class SIM900 {
private:

	bool testGSM();

	bool response(const char* expect, int wait);
	bool command(const __FlashStringHelper *command);
	bool command(const __FlashStringHelper *command, const char* expect);
	bool command(const __FlashStringHelper *command, int wait);
	bool command(const __FlashStringHelper *command, const char* expect, int wait);
	void printResponse(const char* prefix);
	bool getTimeString(char* buf, const int buf_size);
	void readResponseToBuff(char * buf, const int buf_size);
public:

	SoftwareSerial * gsm_modem;
	bool gprs_connected = false;
	bool server_connected = false;
	bool initialized = false;
	SIM900(SoftwareSerial * serial);
	time_t getCurrentTime();
	bool initialize();
	bool connectToGPRS();
	bool connectToServer();
	bool disconnectFromServer();
	void writeJsonToServer(JsonObject* json);
	void printIP();
};

#endif /* SIM900_H_ */
