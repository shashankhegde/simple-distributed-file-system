/*
 * DHCPClient.cpp
 *
 *  Created on: Jan 23, 2011
 *      Author: shashank
 */

#include "DHCPClient.h"
#include <cstring>
#include <cstdio>
#include <arpa/inet.h>
using namespace std;

CDHCPClient::CDHCPClient(char* aDHCPServerIp)
{
	iSocket = NULL;
	iDHCPServerIp = new char[24];
	strcpy(iDHCPServerIp,aDHCPServerIp);
	iDHCPPort = 10001;
}

CDHCPClient::~CDHCPClient()
{

}

unsigned short CDHCPClient::GetClientId(unsigned short aSDFSClientPort)
{
	if(iSocket == NULL)
	{
		iSocket = new SDFSSocket(SDFSSocket::EUDP);
		iSocket->Connect(iDHCPServerIp,iDHCPPort);
	}

	unsigned short port = htons(aSDFSClientPort);
	iSocket->SendTo((const char*)&port,sizeof(unsigned short));

	int bufferLen = 32;
	char* buffer = new char[bufferLen];
	iSocket->RecvFrom(buffer,bufferLen);

	unsigned short id = ntohs(*((unsigned short*)buffer));
	printf("My id : %d", id);
	return id;

}
