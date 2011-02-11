/*
 * DictionaryManager.cpp
 *
 *  Created on: Feb 10, 2011
 *      Author: shashank
 */

#include "DictionaryManager.h"

DictionaryManager::DictionaryManager(int aClientId)
{
	iClientId = aClientId;
	iTimeTable = new TimeTable(5,aClientId);
	iDictionary = new Dictionary(aClientId);
}

DictionaryManager::~DictionaryManager()
{
	delete iTimeTable;
	delete iDictionary;
}

int DictionaryManager::Insert(string& aKey, string& aValue)
{
	int time = iClock.GetTime();
	iDictionary->Insert(aKey,aValue,time);
	iTimeTable->UpdateTable(iClientId,iClientId,time);

	return 0;
}

int DictionaryManager::Delete(string& aKey)
{
/*
	int time = iClock.GetTime();
	iDictionary->Delete(aKey,time,iClientId);
	iTimeTable->UpdateTable(iClientId,iClientId,time);
*/

	return 0;
}

int DictionaryManager::Lookup(string& aKey)
{
	return 0;
}
