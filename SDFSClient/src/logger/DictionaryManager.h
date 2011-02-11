/*
 * DictionaryManager.h
 *
 *  Created on: Feb 10, 2011
 *      Author: shashank
 */

#ifndef DICTIONARYMANAGER_H_
#define DICTIONARYMANAGER_H_

#include "EventClock.h"
#include "TimeTable.h"
#include "Dictionary.h"

class DictionaryManager
{
public:
	DictionaryManager(int aClientId);
	~DictionaryManager();

	int Insert(string& aKey, string& aValue);
	int Delete(string& aKey);
	int Lookup(string& aKey);

private:
	int iClientId;
	EventClock iClock;
	TimeTable* iTimeTable;
	Dictionary* iDictionary;
};

#endif /* DICTIONARYMANAGER_H_ */
