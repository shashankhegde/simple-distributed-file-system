/*
 * SDFSControllerUI.cpp
 *
 *  Created on: Jan 22, 2011
 *      Author: shashank
 */

#include "SDFSControllerUI.h"
#include "SDFSDebug.h"
#include "SDFSClient.h"
#include "logger/DictionaryEvent.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
using namespace std;

SDFSControllerUI::SDFSControllerUI(SDFSClient* aClient)
{
	iClient = aClient;
	iMsgBufferSize = 1024;
	iMsgBuffer = new char[iMsgBufferSize];
}

SDFSControllerUI::~SDFSControllerUI()
{
	delete[] iMsgBuffer;
}

void SDFSControllerUI::Run()
{
	char opt[5];
	bool doContinue = true;
	while(doContinue)
	{
		cout << "\n\n Simple Distributed File System\n";
		if(iClient==0)
			cout << "-------------FILE SERVER------------\n";
		cout << "--------------------------------\n";
		cout << "1. Send Message\n";
		cout << "2. Write data\n";
		cout << "3. Insert\n";
		cout << "4. Delete\n";
		cout << "5. List Keys\n";
		cout << "6. Lookup\n";
		cout << "7. TimeTable\n";
		cout << "0. Exit\n\n";
		cout << "-> ";

		cin >> opt;
		switch(atoi(opt))
		{
		case 1:
			GetMessage();
			break;
		case 2:
			WriteData();
			break;
		case 3:
			InsertKey();
			break;
		case 4:
			DeleteKey();
			break;
		case 5:
			ListKeys();
			break;
		case 6:
			LookupKey();
			break;
		case 7:
			ShowTimeTable();
			break;
		case 0:
			doContinue = false;
			break;
		default:
			CDebug::Log("Unknown option");
		}
	}
}

void SDFSControllerUI::GetMessage()
{
	cout << "Destination id : ";
	char dest[10] = {0};
	cout.flush();
	// TODO: Figure out how to flush out '\n'
	cin.getline(dest,10); //iMsgBuffer,iMsgBufferSize);
	cin.getline(dest,10); //iMsgBuffer,iMsgBufferSize);
	unsigned short destId = atoi(dest);

	cout << "Enter message (delimiter : new line) : ";
	cin.getline(iMsgBuffer,iMsgBufferSize);

	unsigned long dataLen = strlen(iMsgBuffer)+1;
	char* data = new char[dataLen];
	memcpy(data,iMsgBuffer,dataLen);

	SendMessage(destId,dataLen,data,EString);
}

void SDFSControllerUI::WriteData()
{
	unsigned short destId = 0; // File Server is client 0
	getchar();
	cout << "Enter message (delimiter : new line) : ";
	cin.getline(iMsgBuffer,iMsgBufferSize);

	unsigned long dataLen = strlen(iMsgBuffer);
	char* data = new char[dataLen];
	memcpy(data,iMsgBuffer,dataLen);

	SendMessage(destId,dataLen,data,EAppendData);
}

void SDFSControllerUI::SendMessage(unsigned short aClientId, unsigned long aMessageLength, void* aMessage, TMessageType aMessageType)
{
	//cout << "SendMessage() ++, " << (char*)aMessage << endl;
	SDFSMessage* msg = new SDFSMessage();
	msg->SetDestination(aClientId);
	msg->SetSource(iClient->GetId());
	msg->SetMessageType(aMessageType);
	msg->SetMessageId(0);
	msg->SetMessageData((const char*)aMessage,aMessageLength);

	iClient->SendMessage(msg);
	//cout << "SendMessage() --";
}

void SDFSControllerUI::InsertKey()
{
	string keyValue;
	string key;
	string value;
	cout << "Enter Key:Value pair : ";
	cin  >> keyValue;

	int pos = keyValue.find(':');
	key = keyValue.substr(0,pos);
	value = keyValue.substr(pos+1,keyValue.length());

	cout << "Inserting " << key << " : " << value << endl;

	DictionaryEntry de(key,value);
	DictionaryEvent e(DictionaryEvent::EInsert,iClient->GetId(),iClient->GetTime(),&de);

	char* data = e.Serialize();
	int   dataLen = e.SerializedDataLength();

	SendMessage(iClient->GetId(),dataLen,data,EDictionaryEvent);
}

void SDFSControllerUI::DeleteKey()
{
	string key;
	string value="";
	cout << "Enter Key : ";
	cin  >> key;

	DictionaryEntry de(key,value);
	DictionaryEvent e(DictionaryEvent::EDelete,iClient->GetId(),iClient->GetTime(),&de);

	char* data = e.Serialize();
	int   dataLen = e.SerializedDataLength();

	SendMessage(iClient->GetId(),dataLen,data,EDictionaryEvent);
}

void SDFSControllerUI::LookupKey()
{
	string key;
	string value="";
	cout << "Enter Key : ";
	cin  >> key;
	iClient->Lookup(key);
}

void SDFSControllerUI::ListKeys()
{
	iClient->ListKeys();
}

void SDFSControllerUI::ShowTimeTable()
{
	iClient->ShowTimeTable();
}

void SDFSControllerUI::Sync()
{
}
