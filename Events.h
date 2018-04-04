/*
 * events.h
 *
 *  Created on: Apr 4, 2018
 *      Author: erki
 */

#include "Event.h"

#ifndef EVENTS_H_
#define EVENTS_H_

#define EVENTS_START_ADDRESS 100
#define EVENT_OFFSET_ADDRESS 99
#define EVENTS_COUNT 10

/*
 EVENT: (1 byte + 4 bytes) = 5 bytes

 1) EVENT NUMBER
 2) SECONDS FROM Jan 1 1970 (unsigned long)

 erki on nunnu
 */

class Events {
private:
	unsigned char last_event_eeprom_offset;
	void clearEventsLog();
public:
	Events(bool first_boot);
	Event getLastEvent();
	Event getEventByOffset(unsigned char offset);
	void writeNewEvent(Event event);
	void printAllEvents();
};



#endif /* EVENTS_H_ */
