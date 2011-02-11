/*
 * TimeTable.h
 *
 *  Created on: Feb 4, 2011
 *      Author: shashank
 */

#ifndef TIMETABLE_H_
#define TIMETABLE_H_

#include "DictionaryEvent.h"

class TimeTable
{
public:
	static TimeTable* Initialize(const unsigned char* aData, int aDataLen, unsigned short aClientId);

	TimeTable(int aClientCount, unsigned short aClientId);
	~TimeTable();

	int UpdateTable(unsigned short aNode, unsigned short aNodeTarget, unsigned long aTime);
	int UpdateTable(TimeTable& aTimeTable);

	char* Serialize();
	void  DeSerialize(char* aData, int aDataLength);
	int   SerializedDataLength();

	// Do I know that aClient knows about aEvent?
	bool HasRecord(DictionaryEvent& aEvent, int aClient) const;

	void Show();

private:
	unsigned long iClientCount;
	unsigned short iClientId;
	unsigned long* iTimeTable;
};

#endif /* TIMETABLE_H_ */
