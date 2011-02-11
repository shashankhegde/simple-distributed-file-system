/*
 * Logger.cpp
 *
 *  Created on: Feb 7, 2011
 *      Author: shashank
 */

#include "Logger.h"
#include <fstream>
#include <cstring>
#include <iostream>
#include <cstdlib>
using namespace std;

Logger::Logger(const char* aFileName)
{
	int len = strlen(aFileName);
	iFileName = new char[len+1];
	strcpy(iFileName,aFileName);
}

Logger::~Logger()
{
	delete[] iFileName;
}

void Logger::AppendLog(DictionaryEvent& aEvent)
{
	ofstream fout(iFileName,ios::app);
	aEvent.Write(fout);
	fout.close();
}

void Logger::InsertLog(DictionaryEvent& aEvent)
{
	//cout << "Inserting " << aEvent.Node() << " " << aEvent.Time() << endl;

	ofstream fout("tmp");
	ifstream fin(iFileName);
	if(fin.fail())
	{
		AppendLog(aEvent);
		fout.close();
		return;
	}

	DictionaryEvent* e = DictionaryEvent::ReadEvent(fin);
	while(e)
	{
		if(e->HappenedBefore(aEvent))
		{
			e->Write(fout);
		}
		else
		{
			aEvent.Write(fout);
			e->Write(fout);
			break;
		}

		delete e;
		e = DictionaryEvent::ReadEvent(fin);
	}

	// copy remaining events as is
	if(e)
	{
		// was inserted

		while(e)
		{
			e = DictionaryEvent::ReadEvent(fin);
			if(e)
				e->Write(fout);
		}
	}
	else
	{
		// was not inserted as this event is the latest event
		aEvent.Write(fout);
	}

	fout.close();
	fin.close();

	// copy back to iFileName
	fin.open("tmp");
	fout.open(iFileName);
	int bufferSize = 1024;
	char* buffer = new char[bufferSize];
	int read=0;
	while( !fin.eof() )
	{
		fin.read(buffer,bufferSize);
		read = fin.gcount();
		fout.write(buffer,read);
	}
	delete buffer;
	buffer = NULL;
	fout.close();
	fin.close();

	//cout << aEvent.Node() << " " << aEvent.Time() << " inserted!\n";
}

void Logger::DeleteLog(DictionaryEvent& aEvent)
{
}

bool Logger::Deleted(string& aKey)
{
	ifstream fin(iFileName);
	if(fin.fail())
	{
		return false;
	}

	DictionaryEvent* e = DictionaryEvent::ReadEvent(fin);
	while(e)
	{
		if(e->GetDictionaryEntry()->Key() == aKey && e->Type() == DictionaryEvent::EDelete)
			return true;

		delete e;
		e = DictionaryEvent::ReadEvent(fin);
	}

	return false;
}

list<DictionaryEvent*> Logger::GetLogs(int aForNode, const TimeTable* aTimeTable)
{
	list<DictionaryEvent*> logs;

	ifstream fin(iFileName);
	if(fin.fail())
	{
		return logs;
	}

	DictionaryEvent* e = DictionaryEvent::ReadEvent(fin);
	while(e)
	{
		if(!aTimeTable->HasRecord(*e,aForNode))
			logs.push_back(e);

		e = DictionaryEvent::ReadEvent(fin);
	}

	return logs;
}
