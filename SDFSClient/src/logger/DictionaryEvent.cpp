/*
 * Event.cpp
 *
 *  Created on: Feb 7, 2011
 *      Author: shashank
 */

#include "DictionaryEvent.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <cassert>

using namespace std;

/*
Event::Event(TEventType aType, unsigned long aTime, unsigned short aNode)
{
	iType = aType;
	iTime = aTime;
	iNode = aNode;
}
*/

DictionaryEvent::DictionaryEvent(TEventType aType, unsigned short aNode, unsigned long aTime, DictionaryEntry* aDictionaryEntry)
{
	iType = aType;
	iNode = aNode;
	iTime = aTime;
	iDictionaryEntry = aDictionaryEntry;
}

DictionaryEvent::TEventType DictionaryEvent::Type()
{
	return iType;
}

unsigned long DictionaryEvent::Time()
{
	return iTime;
}

unsigned short DictionaryEvent::Node()
{
	return iNode;
}

void DictionaryEvent::Write(ostream& out)
{
	// cout << "Event: DE : 0x" << iDictionaryEntry << endl;

	assert(iDictionaryEntry);

	out << "<event>";
	out << "<id=" << iNode << "/>";
	out << "<op=" << iType << "/>";
	out << "<time=" << iTime << "/>";
	iDictionaryEntry->Write(out);
	out << "</event>" << endl;
}

DictionaryEvent* DictionaryEvent::ReadEvent(istream& fin)
{
	if(fin.eof())
		return NULL;

	if(!ParseEventStart(fin))
	{
		return NULL;
	}

	int id;
	if(!ParseValue(fin,id))
	{
		return NULL;
	}

	int type;
	if(!ParseValue(fin,type))
	{
		return NULL;
	}

	int time;
	if(!ParseValue(fin,time))
	{
		return NULL;
	}

	DictionaryEntry* d = DictionaryEntry::Read(fin);

	if(!ParseEventEnd(fin))
	{
		return NULL;
	}

	DictionaryEvent* e = new DictionaryEvent((DictionaryEvent::TEventType)type,id,time,d);
	return e;
}


bool DictionaryEvent::ParseEventStart(istream& fin)
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

bool DictionaryEvent::ParseEventEnd(istream& fin)
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

bool DictionaryEvent::ParseValue(istream& fin, int& aValue)
{
	int bufferSize = 1024;
	char* buffer = new char[bufferSize];
	fin.getline(buffer,bufferSize,'>');
	//cout << buffer << endl;

	char* s = strtok(buffer,"=");
	s = strtok(NULL,"/");
	//cout << s << endl;
	int i;
	if(s)
		i = atoi(s);

	aValue = i;
	//cout << s << endl;

	fin.getline(buffer,bufferSize,'<');
	fin.putback('<');
	delete[] buffer;

	return true;
}

/*
bool Event::ParseId(ifstream& fin, unsigned short& aId)
{
	int bufferSize = 1024;
	char* buffer = new char[bufferSize];
	fin.getline(buffer,bufferSize,'>');
	//cout << buffer << endl;

	char* s = strtok(buffer,"=");
	s = strtok(NULL,"/");
	aId = atoi(s);
	cout << s << endl;

	fin.getline(buffer,bufferSize,'<');
	fin.putback('<');
	delete[] buffer;

	return true;
}

bool Event::ParseType(ifstream& fin, int& aType)
{
	int bufferSize = 1024;
	char* buffer = new char[bufferSize];
	fin.getline(buffer,bufferSize,'>');
	//cout << buffer << endl;

	char* s = strtok(buffer,"=");
	s = strtok(NULL,"/");
	aType = atoi(s);
	cout << s << endl;

	fin.getline(buffer,bufferSize,'<');
	fin.putback('<');
	delete[] buffer;

	return true;
}

bool Event::ParseTime(ifstream& fin, unsigned long& aTime)
{
	int bufferSize = 1024;
	char* buffer = new char[bufferSize];
	fin.getline(buffer,bufferSize,'>');
	//cout << buffer << endl;

	char* s = strtok(buffer,"=");
	s = strtok(NULL,"/");
	aTime = atoi(s);
	cout << s << endl;

	fin.getline(buffer,bufferSize,'<');
	fin.putback('<');
	delete[] buffer;

	return true;
}
*/


bool DictionaryEvent::HappenedBefore(DictionaryEvent& aEvent)
{
	if(iTime < aEvent.Time())
		return true;
	else if( iTime == aEvent.Time() && iNode < aEvent.Node())
		return true;
	else
		return false;
}

char* DictionaryEvent::Serialize()
{
	stringstream ss;
	Write(ss);

	char* data = new char[ss.str().length()+1];
	strcpy(data,ss.str().c_str());

	return data;
}

int DictionaryEvent::SerializedDataLength()
{
	stringstream ss;
	Write(ss);
	return ss.str().length();
}

void DictionaryEvent::DeSerialize(char* aData, int aDataLength)
{
	istringstream iss;
	iss.str(aData);

}

DictionaryEntry* DictionaryEvent::GetDictionaryEntry()
{
	return iDictionaryEntry;
}
