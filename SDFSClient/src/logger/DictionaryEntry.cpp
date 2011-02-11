/*
 * DictionaryEntry.cpp
 *
 *  Created on: Feb 8, 2011
 *      Author: shashank
 */

#include "DictionaryEntry.h"
#include <cstring>
#include <cstdlib>
#include <iostream>
using namespace std;

DictionaryEntry::DictionaryEntry(string& aKey, string& aValue)
{
/*
	iNode = aNode;
	iTime = aTime;
*/
	iKey = aKey;
	iValue = aValue;
}

DictionaryEntry::~DictionaryEntry()
{

}

/*
int DictionaryEntry::Node()
{
	return 0; //iNode;
}
*/

/*
int DictionaryEntry::Time()
{
	return 0; //iTime;
}
*/

string& DictionaryEntry::Key()
{
	return iKey;
}

string& DictionaryEntry::Value()
{
	return iValue;
}

DictionaryEntry* DictionaryEntry::Read(istream& fin)
{
	//cout << "DictionaryEntry::Read() ++\n";
	if(fin.eof())
		return NULL;

	if(!ParseEntryStart(fin))
	{
		return NULL;
	}

/*
	int id;
	if(!ParseValue(fin,id))
	{
		return NULL;
	}

	int time;
	if(!ParseValue(fin,time))
	{
		return NULL;
	}
*/

	string key;
	if(!ParseValue(fin,key))
	{
		return NULL;
	}

	//cout << "Read key : " << key << endl;

	string value;
	if(!ParseValue(fin,value))
	{
		return NULL;
	}

	//cout << "Read Value: " << value << endl;

	if(!ParseEntryEnd(fin))
	{
		return NULL;
	}

	//cout << "Read Entry\n";

	DictionaryEntry* e = new DictionaryEntry(key,value);
	//cout << "DictionaryEntry::Read() --\n";
	return e;
}

int DictionaryEntry::Write(ostream& out)
{
	out << "<entry>";
/*
	out << "<node=" << iNode << ">";
	out << "<time=" << iTime << ">";
*/
	out << "<key=" << iKey << ">";
	out << "<value=" << iValue << ">";
	out << "</entry>";

	return 0;
}


bool DictionaryEntry::ParseEntryStart(istream& fin)
{
	int bufferSize = 1024;
	char* buffer = new char[bufferSize];
	fin.getline(buffer,bufferSize,'>');

	//cout << buffer << endl;

	if(strncmp(buffer,"<entry",6))
	{
		//cout << "Entry not found\n";
		return false;
	}

	fin.getline(buffer,bufferSize,'<');
	fin.putback('<');
	delete[] buffer;

	return true;
}

bool DictionaryEntry::ParseEntryEnd(istream& fin)
{
	int bufferSize = 1024;
	char* buffer = new char[bufferSize];
	fin.getline(buffer,bufferSize,'>');
	//cout << buffer << endl;
	fin.getline(buffer,bufferSize,'<');
	fin.putback('<');
	delete[] buffer;

	return true;
}

bool DictionaryEntry::ParseValue(istream& fin, int& aId)
{
	int bufferSize = 1024;
	char* buffer = new char[bufferSize];
	fin.getline(buffer,bufferSize,'>');
	//cout << buffer << endl;

	char* s = strtok(buffer,"=");
	s = strtok(NULL,"/");
	aId = atoi(s);
	//cout << s << endl;

	fin.getline(buffer,bufferSize,'<');
	fin.putback('<');
	delete[] buffer;

	return true;
}

bool DictionaryEntry::ParseValue(istream& fin, string& aStringVal)
{
	int bufferSize = 1024;
	char* buffer = new char[bufferSize];
	fin.getline(buffer,bufferSize,'>');
	//cout << buffer << endl;

	char* s = strtok(buffer,"=");
	s = strtok(NULL,"/");
	if(s)
		aStringVal = s;
	else
		aStringVal = "";
	//cout << aStringVal << endl;

	fin.getline(buffer,bufferSize,'<');
	fin.putback('<');
	delete[] buffer;

	return true;
}

