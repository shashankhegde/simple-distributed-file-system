/*
 * GFSListener.cpp
 *
 *  Created on: Jan 16, 2011
 *      Author: shashank
 */

#include "SDFSListener.h"
#include "SDFSDebug.h"
#include "SDFSMessage.h"
#include <cstring>
#include <cassert>
#include "Utils.h"

// initialize static variables
SDFSListener* SDFSListener::iInstance = NULL;

SDFSListener* SDFSListener::Instance(SDFSClient* aClient, unsigned short aPort)
{
	return new SDFSListener(aClient, aPort);

/*
 * Should this be a singleton object?
 *
	if(iInstance == NULL)
	{
		iInstance = new CGFSListener(aPort);
	}

	return iInstance;
*/
}

SDFSListener::SDFSListener(SDFSClient* aClient, unsigned short aPort)
{
	iPort = aPort;
	iSocket = NULL;
	iListening = false;
	iClient = aClient;
}

SDFSListener::~SDFSListener()
{

}

int SDFSListener::Start()
{
	if(iListening)
	{
		CDebug::Error("Already listening!");
		return -1;
	}

	int ret = pthread_create(&iListenerThread, NULL, ListenerThreadSpawner, this);
	if(ret != 0)
	{
		CDebug::Error("Couldn't start listener thread");
	}
	return ret;
}

int SDFSListener::Stop()
{
	// kill the thread
	iListening = false;
	return 0;
}

void* SDFSListener::ListenerThreadSpawner(void* aGFSListener)
{
	SDFSListener* gfsListener = (SDFSListener*)aGFSListener;
	if(gfsListener == NULL)
		return 0;

	return gfsListener->ListenerThread(NULL);
}

void* SDFSListener::ListenerThread(void* aData)
{
	iListening = true;

	assert(iSocket == NULL);

	iSocket = new SDFSSocket(SDFSSocket::EUDP);
	if(iSocket == NULL)
	{
		iListening = false;
		return NULL;
	}

	iSocket->Listen(iPort);

	while(true) // add a flag check
	{
		// wait for a connection and then accept it
		//SDFSSocket* newSocket = iSocket->Accept();

		ReceiveMessage();


		//HandleMessage(buffer, rcvdData);

		// spawn a handler for an incoming connection
		//HandleConnection(newSocket);
	}

	return 0;
}

/*
 *   |--------------------------------------------------------------------------------------|
 *   | DestClientId (2) | SrcClientId (2) | MsgType (2) | MsgId (4) | DataLen (4) | MsgData |
 *   |--------------------------------------------------------------------------------------|
 */

void SDFSListener::ReceiveMessage()
{
	unsigned int bufferLen = 2048;
	char* buffer = new char[bufferLen];

	iSocket->RecvFrom(buffer,bufferLen);

	SDFSMessage* msg = new SDFSMessage();

	// dest id
	//iSocket->RecvFrom(buffer, bufferLen);
	unsigned char* ptrToFields = (unsigned char*)buffer;
	msg->SetDestination(UnpackShort(ptrToFields));
	ptrToFields+=2;

	// src id
	//iSocket->RecvFrom(buffer, bufferLen);
	msg->SetSource(UnpackShort(ptrToFields));
	ptrToFields+=2;

	// msg type
	// iSocket->RecvFrom(buffer, bufferLen);
	msg->SetMessageType((TMessageType)UnpackShort(ptrToFields));
	ptrToFields+=2;

	// msg id
	//iSocket->RecvFrom(buffer, bufferLen);
	msg->SetMessageId(UnpackLong(ptrToFields));
	ptrToFields+=4;

	// data len
	//iSocket->RecvFrom(buffer, bufferLen);
	unsigned long dataLen = UnpackLong(ptrToFields);
	ptrToFields+=4;

	printf("Data Length : %u, bufferLen = %u\n",dataLen, bufferLen);

	char* data = NULL;
	if(dataLen < bufferLen)
	{
		data = new char[dataLen];
	}
	else
	{
		printf("Data Length : %u, bufferLen = %u\n",dataLen, bufferLen);
		CDebug::Error("Data larger than buffer!");
	}

	// data
	//iSocket->RecvFrom(buffer, bufferLen);
	memcpy(data,ptrToFields,dataLen);

	msg->SetMessageData((char*)data, dataLen);

	if(iClient)
	{
		iClient->ReceivedMessage(msg);
	}

}

int SDFSListener::HandleConnection(SDFSSocket* aSocket)
{
	return 0;
}
