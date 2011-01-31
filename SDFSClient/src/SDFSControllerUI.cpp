/*
 * SDFSControllerUI.cpp
 *
 *  Created on: Jan 22, 2011
 *      Author: shashank
 */

#include "SDFSControllerUI.h"
#include "SDFSDebug.h"
#include "SDFSClient.h"

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
	char opt;
	bool doContinue = true;
	while(doContinue)
	{
		cout << "\n\n Simple Distributed File System\n";
		if(iClient==0)
			cout << "-------------FILE SERVER------------\n";
		cout << "--------------------------------\n";
		cout << "1. Send Message\n";
		cout << "2. Write data\n";
		cout << "0. Exit\n\n";
		cout << "-> ";

		opt = cin.get();
		switch(opt)
		{
		case '1':
			GetMessage();
			break;
		case '2':
			WriteData();
			break;
		case '0':
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
	cout << "SendMessage() ++, " << (char*)aMessage << endl;
	SDFSMessage* msg = new SDFSMessage();
	msg->SetDestination(aClientId);
	msg->SetSource(iClient->GetId());
	msg->SetMessageType(aMessageType);
	msg->SetMessageId(0);
	msg->SetMessageData((const char*)aMessage,aMessageLength);

	iClient->SendMessage(msg);
	cout << "SendMessage() --";
}
