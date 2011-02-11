/*
 * DNSClient.cpp
 *
 *  Created on: Jan 22, 2011
 *      Author: shashank
 */

#include "DNSClient.h"
#include "SDFSDebug.h"

#include <cstring>
#include <cstdio>
using namespace std;

CDNSClient* CDNSClient::iDNSClient;
int CDNSClient::iInstanceCount = 0;

CDNSClient* CDNSClient::Instance(char* aDNSServerIp)
{
	if(!iDNSClient)
	{
		iDNSClient = new CDNSClient(aDNSServerIp);
	}

	iInstanceCount++;
	return iDNSClient;
}

CDNSClient::CDNSClient(char* aDNSServerIp)
{
	iDNSServerIp = new char[16];
	strcpy(iDNSServerIp,aDNSServerIp);

	iDNSPort = 10002;
	iSocket = 0;
}

void CDNSClient::Delete()
{
	iInstanceCount--;
	if(iInstanceCount == 0)
	{
		delete iDNSClient;
		iDNSClient = NULL;
	}
}

CDNSClient::~CDNSClient()
{
	delete[] iDNSServerIp;
	if(iSocket)
		delete iSocket;
}

TDNSInfo* CDNSClient::GetIPAddr(unsigned short aClientId)
{
	CDebug::Log("DNSClient : Getting IP addr");

	if(!iSocket)
	{
		iSocket = new SDFSSocket(SDFSSocket::EUDP);
		iSocket->Connect(iDNSServerIp,iDNSPort);
	}

	iSocket->SendTo((char*)&aClientId,sizeof(unsigned short));

	// expected data :
	// 4 bytes - 4 ip octects followed by 2 bytes of port number
	char buffer[32];
	iSocket->RecvFrom(buffer,6);

	TDNSInfo* info = new TDNSInfo;
	sprintf(info->iIP,"%d.%d.%d.%d",buffer[0],buffer[1],buffer[2],buffer[3]);
	char* ptrToPort = buffer+4;
	unsigned short port = *((unsigned short*)ptrToPort);
	info->iPort = port;
	printf("DNS Data Received : %s:%x",info->iIP,info->iPort);

	return info;
}
