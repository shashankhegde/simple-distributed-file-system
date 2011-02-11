/*
 * Dictionary.cpp
 *
 *  Created on: Feb 7, 2011
 *      Author: shashank
 */

#include "Dictionary.h"
#include <sstream>
#include <cstdlib>
#include <iostream>

using namespace std;

Dictionary::Dictionary(int aNode)
{
	system("mkdir logs");

	iNode = aNode;
	iDictionaryFile = "logs/dictionary_";
	stringstream ss;
	ss << iNode;
	iDictionaryFile += ss.str();
	iTempFile = "logs/dict_tmp_";
	iTempFile += ss.str();

	string logFileName = "logs/log_";
	logFileName += ss.str();

	iLogger = new Logger(logFileName.c_str());

}

Dictionary::~Dictionary()
{

}

int Dictionary::Insert(DictionaryEvent* aDictionaryEvent)
{
	DictionaryEntry* de = aDictionaryEvent->GetDictionaryEntry();
	if(!de)
		return -1;

	//cout << "Inserting " << de->Key() << " , " << de->Value() << endl;

	if(iLogger->Deleted(de->Key()))
		return -1;

	// just append to the file
	ofstream fout(iDictionaryFile.c_str(),ios::app);
	if(de)
	{
		//cout << "de : 0x" << de << endl;
		iLogger->InsertLog(*aDictionaryEvent);
		if(de)
			de->Write(fout);
		fout << endl;
	}

	fout.close();
	//cout << "Successfully inserted " << de->Key() << " , " << de->Value() << endl;
	return 0;
}

int Dictionary::Delete(DictionaryEvent* aDictionaryEvent)
{
	//cout << "Dictionary::Delete(DictionaryEvent* aDictionaryEvent) ++ \n";

	DictionaryEntry* de = aDictionaryEvent->GetDictionaryEntry();
	int ret = Delete(de->Key(),aDictionaryEvent->Time(),aDictionaryEvent->Node());

	//cout << "Dictionary::Delete(DictionaryEvent* aDictionaryEvent) -- \n";
	//return Insert(de->Key(),de->Value(),aDictionaryEvent->Time());

	return ret;
}

int Dictionary::Insert(string& aKey, string& aValue, int aTime)
{
	//cout << "Inserting " << aKey << " , " << aValue << endl;

	if(iLogger->Deleted(aKey))
		return -1;

	// just append to the file
	ofstream fout(iDictionaryFile.c_str(),ios::app);
	DictionaryEntry* de = new DictionaryEntry(aKey,aValue);
	if(de)
	{
		//cout << "de : 0x" << de << endl;
		DictionaryEvent* e = new DictionaryEvent(DictionaryEvent::EInsert,iNode,aTime,de);
		iLogger->InsertLog(*e);
		delete e;
		de->Write(fout);
		delete de;
		fout << endl;
	}

	fout.close();
	//cout << "Successfully inserted " << aKey << " , " << aValue << endl;
	return 0;
}

int Dictionary::Delete(string& aKey, int aTime, int aNode)
{
	ifstream fin(iDictionaryFile.c_str());
	ofstream ftmp(iTempFile.c_str());
	DictionaryEntry* de = DictionaryEntry::Read(fin);
	while(de)
	{
		if(de->Key() != aKey)
		{
			de->Write(ftmp);
			ftmp << endl;
		}
		else
		{
			DictionaryEvent* e = new DictionaryEvent(DictionaryEvent::EDelete,aNode,aTime,de);
			iLogger->InsertLog(*e);
			delete e;
		}

		delete de;
		de = DictionaryEntry::Read(fin);
	}
	ftmp.close();
	fin.close();

	// copy back to the dictionary file
	fin.open(iTempFile.c_str(),ios::binary);
	ofstream fout(iDictionaryFile.c_str(),ios::binary);
	char buffer[1024];
	while(!fin.eof())
	{
		fin.read(buffer,1024);
		fout.write(buffer, fin.gcount());
	}
	fin.close();
	fout.close();
	return 0;
}

int Dictionary::Lookup(string& aKey)
{
	ifstream fin(iDictionaryFile.c_str());
	DictionaryEntry* de = DictionaryEntry::Read(fin);
	while(de)
	{
		if(de->Key() != aKey)
		{
			delete de;
			de = DictionaryEntry::Read(fin);
		}
		else
		{
			cout << aKey << " : " << de->Value() << " present in local copy\n";
			fin.close();
			return 0;
		}

	}
	fin.close();
	return -1;
}

int Dictionary::ListKeys()
{
	ifstream fin(iDictionaryFile.c_str());
	DictionaryEntry* de = DictionaryEntry::Read(fin);
	while(de)
	{
		cout << de->Key() << endl;
		de = DictionaryEntry::Read(fin);
	}

	fin.close();
	return -1;
}

list<DictionaryEvent*> Dictionary::GetLogs(int aForNode, const TimeTable* aTimeTable)
{
	return iLogger->GetLogs(aForNode,aTimeTable);
}

