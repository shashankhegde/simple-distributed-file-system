/*
 * Event.h
 *
 *  Created on: Feb 4, 2011
 *      Author: shashank
 */

#ifndef EVENT_H_
#define EVENT_H_

#include "DictionaryEntry.h"
#include <fstream>
using namespace std;

class DictionaryEvent
{
public:
	enum TEventType { EInsert, EDelete };

public:
//	Event(TEventType aType, unsigned long aTime, unsigned short aNode);
	DictionaryEvent(TEventType aType, unsigned short aNode, unsigned long aTime,DictionaryEntry* aDictionaryEntry);
	static DictionaryEvent* ReadEvent(istream& fin);

	TEventType Type();
	unsigned long Time();
	unsigned short Node();
	void Write(ostream& out);
	bool HappenedBefore(DictionaryEvent& aEvent);
	DictionaryEntry* GetDictionaryEntry();

	char* Serialize();
	void  DeSerialize(char* aData, int aDataLength);
	int   SerializedDataLength();


private: // methods
	static bool ParseEventStart(istream& fin);
	static bool ParseEventEnd(istream& fin);
	static bool ParseValue(istream& fin, int& aId);
/*
	static bool ParseId(ifstream& fin, unsigned short& aId);
	static bool ParseType(ifstream& fin, int& aType);
	static bool ParseTime(ifstream& fin, unsigned long& aTime);
*/




private:
	TEventType iType;
	DictionaryEntry* iDictionaryEntry;
	unsigned long iTime;
	unsigned short iNode;
};

#endif /* EVENTRECORD_H_ */
