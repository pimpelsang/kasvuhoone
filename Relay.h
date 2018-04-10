/*
 * Relay.h
 *
 *  Created on: Mar 28, 2018
 *      Author: erki
 */

#ifndef RELAY_H_
#define RELAY_H_

class Relay {
private:
	int digitalPin;
	bool invertedLogic;
	bool state = true;
public:
	bool getState();
	Relay(int digitalPin, bool invertedLogic);
	virtual ~Relay();
	bool activate();
	bool deactivate();
};

#endif /* RELAY_H_ */
