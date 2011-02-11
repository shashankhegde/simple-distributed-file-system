/*
 * DictionaryEntry.h
 *
 *  Created on: Feb 8, 2011
 *      Author: shashank
 */

#ifndef DICTIONARYENTRY_H_
#define DICTIONARYENTRY_H_

#include <string>
#include <ostream>
#include <istream>
using namespace std;

class DictionaryEntry
{
public:
	DictionaryEntry(string& aKey, string& aValue);
	~DictionaryEntry();

	static DictionaryEntry* Read(istream& in);
	int Write(ostream& out);


/*
	int Node();
	int Time();
*/
	string& Key();
	string& Value();

private: // methods
	static bool ParseEntryStart(istream& fin);
	static bool ParseEntryEnd(istream& fin);
	static bool ParseValue(istream& fin, int& aId);
	static bool ParseValue(istream& fin, string& aStringVal);


private:
/*
	int iNode;
	int iTime;
*/
	string iKey;
	string iValue;

};

#endif /* DICTIONARYENTRY_H_ */
