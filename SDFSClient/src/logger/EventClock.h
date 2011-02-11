/*
 * EventClock.h
 *
 *  Created on: Feb 8, 2011
 *      Author: shashank
 */

#ifndef EVENTCLOCK_H_
#define EVENTCLOCK_H_

#include "DictionaryEvent.h"

class EventClock
{
public:
	EventClock()
	{
		iTime = 0;
	}
	~EventClock()
	{

	}

	unsigned int GetTime()
	{
		iTime++;
		return iTime;
	}

	unsigned int GetCurrentTime()
	{
		return iTime;
	}

	unsigned int GetTime(DictionaryEvent& aEvent)
	{
		unsigned int maxTime = (aEvent.Time() > iTime) ? aEvent.Time() : iTime;
		iTime = maxTime + 1;
		return iTime;
	}

private:
	unsigned int iTime;
};

#endif /* EVENTCLOCK_H_ */
