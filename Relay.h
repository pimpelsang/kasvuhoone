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
	unsigned char digitalPin;
	bool invertedLogic;
	// state has to be true so deactivate can work when initializing Relay
	bool state = true;
public:
	bool getState();
	Relay(unsigned char digitalPin, bool invertedLogic);
	virtual ~Relay();
	bool activate();
	bool deactivate();
};

#endif /* RELAY_H_ */
