/*
 * Logger.h
 *
 *  Created on: Feb 4, 2011
 *      Author: shashank
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "DictionaryEvent.h"
#include "TimeTable.h"
#include <list>
using namespace std;

/*
 * Log Entry:
 *
 * [Time, Pid]
 * 		Event Record
 *
 * [Time, Pid]
 * 		Event Record
 * ...
 *
 */

class Logger
{
public:
	Logger(const char* aFileName);
	~Logger();

	void AppendLog(DictionaryEvent& aEvent);
	void InsertLog(DictionaryEvent& aEvent);
	void DeleteLog(DictionaryEvent& aEvent);	// deletes all events which have occurred earlier than aEvent.Time()

	bool Deleted(string& aKey);
	list<DictionaryEvent*> GetLogs(int aForNode, const TimeTable* aTimeTable);


private:
	DictionaryEvent* ReadEvent(ifstream& fin);
	bool ParseEventStart(ifstream& fin);
	bool ParseEventEnd(ifstream& fin);
	bool ParseId(ifstream& fin, unsigned short& aId);
	bool ParseType(ifstream& fin, int& aType);
	bool ParseTime(ifstream& fin, unsigned long& aTime);

	void CollectGarbage();

private:
	char* iFileName;
};

#endif /* LOGGER_H_ */
