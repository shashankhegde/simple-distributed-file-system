/*
 * SDFSConnector.cpp
 *
 *  Created on: Jan 13, 2011
 *      Author: shashank
 */

#include "SDFSConnector.h"
#include "SDFSDebug.h"
#include "Utils.h"

#include <cstring>
#include <cstdio>
using namespace std;

#ifndef NULL
#define NULL 0
#endif


SDFSConnector::SDFSConnector(unsigned short aRemoteClientId, char* aRouterIp)://, char* aRemoteIp, unsigned short aRemotePort):
	iRemoteClientId(aRemoteClientId),
	iSocket(NULL)
{
	iDNSClient = CDNSClient::Instance(aRouterIp);
	iSocket = new SDFSSocket(SDFSSocket::EUDP);//aRemoteIp,aRemotePort);
	iRouterIp = new char[strlen(aRouterIp)+1];
	strcpy(iRouterIp,aRouterIp);
}

SDFSConnector::~SDFSConnector()
{
	if(iSocket)
		delete iSocket;

	if(iRouterIp)
		delete[] iRouterIp;

}

int SDFSConnector::Send(const SDFSMessage* aMessage)
{
	if(aMessage == NULL)
	{
		CDebug::Error("Trying to send NULL message");
		return -1;
	}

	if(iSocket == NULL)
	{
		CDebug::Error("Socket is NULL");
		return -1;
	}

	if(!iSocket->IsConnected())
	{
/*
		TDNSInfo* dnsInfo = iDNSClient->GetIPAddr(aMessage->GetDestination());
		if(dnsInfo)
		{
			iSocket->Connect(dnsInfo->iIP,dnsInfo->iPort);
			delete dnsInfo;
		}
*/
		iSocket->Connect(iRouterIp,8000);	// send to router
	}

	/*
	 *   |--------------------------------------------------------------------------------------|
	 *   | DestClientId (2) | SrcClientId (2) | MsgType (2) | MsgId (4) | DataLen (4) | MsgData |
	 *   |--------------------------------------------------------------------------------------|
	 */

	// Pack the data and send in one chunk
	unsigned char dataBuffer[2048];
	unsigned char* ptrInBuffer = dataBuffer;
	unsigned short dest = aMessage->GetDestination();
	PackShort(dest,ptrInBuffer);
	ptrInBuffer+=2;

	unsigned short src = aMessage->GetSource();
	PackShort(src,ptrInBuffer);
	ptrInBuffer+=2;

	unsigned short msgType = aMessage->GetMessageType();
	PackShort(msgType,ptrInBuffer);
	ptrInBuffer+=2;

	unsigned long msgId = aMessage->GetMessageId();
	PackLong(msgId,ptrInBuffer);
	ptrInBuffer+=4;

	unsigned long dl = aMessage->GetDataLength();
	PackLong(dl,ptrInBuffer);
	ptrInBuffer+=4;

	memcpy(ptrInBuffer,aMessage->GetMessageData(),aMessage->GetDataLength());

	unsigned long dataLength = 2+2+2+4+4+aMessage->GetDataLength();	// header + datalen
	iSocket->SendTo((char*)dataBuffer,dataLength);
	return 0;

	//////////// Not Used ////////////

/*
	int error = 0;

	unsigned short destId = aMessage->GetDestination();
	error = iSocket->Send((const char*)&destId,sizeof(unsigned short));
	if(error != 0)
	{
		CDebug::Error("Error sending destination id");
		return error;
	}

	unsigned short srcId = aMessage->GetSource();
	error = iSocket->Send((const char*)&srcId,sizeof(unsigned short));
	if(error != 0)
	{
		CDebug::Error("Error sending destination id");
		return error;
	}

	unsigned short msgType = aMessage->GetMessageType();
	error = iSocket->Send((const char*)&msgType,sizeof(unsigned short));
	if(error != 0)
	{
		CDebug::Error("Error sending destination id");
		return error;
	}

	// send message id
	unsigned long msgId = aMessage->GetMessageId();
	error = iSocket->Send((const char*)&msgId,sizeof(unsigned long));
	if(error != 0)
	{
		CDebug::Error("Error sending message id");
		return error;
	}

	// send data length
	unsigned long dataLen = aMessage->GetDataLength();
	error = iSocket->Send((const char*)&dataLen,sizeof(unsigned long));
	if(error != 0)
	{
		CDebug::Error("Error sending data length");
		return error;
	}

	// send data
	error = iSocket->Send(aMessage->GetMessageData(),aMessage->GetDataLength());
	if(error != 0)
	{
		CDebug::Error("Error sending data");
		return error;
	}
*/

	return 0;
}

SDFSMessage* SDFSConnector::Recv()
{
	if(iSocket == NULL)
	{
		CDebug::Error("Socket is NULL");
		return NULL;
	}

	//TODO: Add checks

	char* messageId = new char[sizeof(unsigned long)];
	iSocket->Recv(messageId,sizeof(unsigned long));
	unsigned long iMessageId = *((unsigned long*)messageId);

	char* messageLen = new char[sizeof(unsigned long)];
	iSocket->Recv(messageLen,sizeof(unsigned long));
	unsigned long iMessageLen = *((unsigned long*)messageLen);

	char* messageData = new char[iMessageLen];
	iSocket->Recv(messageData,iMessageLen);

	SDFSMessage* msg = new SDFSMessage();

	msg->SetMessageId(iMessageId);
	msg->SetMessageData(messageData,iMessageLen);

	delete[] messageId;
	delete[] messageLen;
	delete[] messageData;

	return msg;
}
