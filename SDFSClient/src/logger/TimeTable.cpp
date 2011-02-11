/*
 * TimeTable.cpp
 *
 *  Created on: Feb 4, 2011
 *      Author: shashank
 */

#include "TimeTable.h"
#include "../Utils.h"
#include <iostream>
using namespace std;

int MAX(int a, int b)
{
	return (a > b) ? a : b;
}

TimeTable::TimeTable(int aClientCount, unsigned short aClientId)
{
	//cout << "TimeTable : " << aClientCount << endl;

	iClientId = aClientId;
	iClientCount = aClientCount;
	iTimeTable = new unsigned long[iClientCount*iClientCount];

	// fill all entries with 0
	for(int i=0; i<iClientCount; i++)
		for(int j=0; j<iClientCount; j++)
			iTimeTable[i*iClientCount+j] = 0;
}

TimeTable::~TimeTable()
{
	delete[] iTimeTable;
}

TimeTable* TimeTable::Initialize(const unsigned char* aData, int aDataLen, unsigned short aClientId)
{
	unsigned char* dataPtr = (unsigned char*)aData;
	int clientCount = UnpackLong(dataPtr);
	TimeTable* timeTable = new TimeTable(clientCount,aClientId);
	for(int i=0; i<clientCount; i++)
		for(int j=0; j<clientCount; j++)
		{
			dataPtr += sizeof(long);
			timeTable->iTimeTable[i*timeTable->iClientCount+j] = UnpackLong(dataPtr);
		}

	return timeTable;
}

/*
 * aNode, aNodeTarget : [1..iClientCount]
 */
int TimeTable::UpdateTable(unsigned short aNode, unsigned short aNodeTarget, unsigned long aTime)
{
	if(aNode >= iClientCount || aNodeTarget >= iClientCount)
	{
		cout << "Id greater than " << iClientCount << endl;
		return -1;
	}

	/*
	 * This node knows that aNode has learned of all events that occurred at aNodeTarget
	 * till time aTime.
	 */

	int aNodeIndex = (aNode)*iClientCount;
	int aNodeTargetIndex = (aNodeTarget);

	// Update aNodeIndex+aNodeTargetIndex to aTime
	iTimeTable[aNodeIndex+aNodeTargetIndex] = aTime;
	//cout << "iTimeTable[" << aNodeIndex+aNodeTargetIndex << "] = " << aTime << endl;
	return 0;
}

int TimeTable::UpdateTable(TimeTable& aTimeTable)
{
	//cout << "Received timetable : \n";

	for(int i=0; i<iClientCount; i++)
		iTimeTable[iClientId*iClientCount+i] = MAX( iTimeTable[iClientId*iClientCount+i],
													aTimeTable.iTimeTable[aTimeTable.iClientId*aTimeTable.iClientCount+i]);

	for(int i=0; i<iClientCount; i++)
	{
		for(int j=0; j<iClientCount; j++)
		{
			iTimeTable[iClientId*iClientCount+i] = MAX( iTimeTable[iClientId*iClientCount+i],
														aTimeTable.iTimeTable[aTimeTable.iClientId*aTimeTable.iClientCount+i]);
			//cout << aTimeTable.iTimeTable[i*iClientCount+j] << " - ";
		}
		//cout << "\n";
	}

	return 0;
}

bool TimeTable::HasRecord(DictionaryEvent& aEvent, int aClient) const
{
	int aNodeIndex = (aClient)*iClientCount;
	int aNodeTargetIndex = (aEvent.Node());

	//cout << "iTimeTable[" << aClient << "," << aNodeTargetIndex << "] , " << iTimeTable[aNodeIndex+aNodeTargetIndex] << " : " << aEvent.Time() << endl;

	if(iTimeTable[aNodeIndex+aNodeTargetIndex] >= aEvent.Time())
		return true;

	return false;
}

char* TimeTable::Serialize()
{
	// first word is the client count
	char* data = new char[sizeof(long) + iClientCount*iClientCount*sizeof(long)];
	unsigned char* dataPtr = (unsigned char*)data;

	PackLong(iClientCount,dataPtr);

	for(int i=0; i<iClientCount; i++)
	{
		for(int j=0; j<iClientCount; j++)
		{
			dataPtr += sizeof(long);
			PackLong(iTimeTable[i*iClientCount+j],dataPtr);
			// cout << iTimeTable[i*iClientCount+j] << " - ";
		}
		cout << endl;
	}

	return data;
}

int TimeTable::SerializedDataLength()
{
	return sizeof(long) + iClientCount*iClientCount*sizeof(long);
}

void TimeTable::Show()
{
	for(int i=0; i<iClientCount; i++)
	{
		for(int j=0; j<iClientCount; j++)
		{
			cout << iTimeTable[i*iClientCount+j] << " - ";
		}
		cout << endl;
	}

}
