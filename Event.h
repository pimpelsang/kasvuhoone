/*
 * event.h
 *
 *  Created on: Apr 4, 2018
 *      Author: erki
 */

#include <Time.h>

#ifndef EVENT_H_
#define EVENT_H_

#define EVENT_SIZE_BYTES 5

class Event {
private:
public:
	Event(unsigned char event_number, time_t event_time);
	unsigned char event_number;
	time_t event_time;
	void printEventString();
	void getEventString(char *buf, const int buf_size);
};

#endif /* EVENT_H_ */
