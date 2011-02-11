/*
 * Dictionary.h
 *
 *  Created on: Feb 7, 2011
 *      Author: shashank
 */

#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include "TimeTable.h"
#include "EventClock.h"
#include "Logger.h"
#include <string>
#include <list>

using namespace std;

class Dictionary
{
public:
	Dictionary(int aNode);
	~Dictionary();

	int Insert(DictionaryEvent* aDictionaryEvent);
	int Delete(DictionaryEvent* aDictionaryEvent);

	int Insert(string& aKey, string& aValue, int aTime);
	int Delete(string& aKey, int aTime, int aNode);
	int Lookup(string& aKey);
	int ListKeys();

	list<DictionaryEvent*> GetLogs(int aForNode, const TimeTable* aTimeTable);

private:
	string iDictionaryFile;
	string iTempFile;

	Logger* iLogger;
	int iNode;
};

#endif /* DICTIONARY_H_ */
