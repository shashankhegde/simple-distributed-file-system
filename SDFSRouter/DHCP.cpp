/*
 * DHCP.cpp
 *
 *  Created on: Jan 23, 2011
 *      Author: shashank
 */

#include "DHCP.h"
#include "SDFSDebug.h"
#include <arpa/inet.h>

CDHCP::CDHCP(unsigned short aDHCPPort, CDNS* aDNSServer)
{
	iDHCPPort = aDHCPPort;
	iSocket = new SDFSSocket(SDFSSocket::EUDP);
	iDNSServer = aDNSServer;
}

CDHCP::~CDHCP()
{

}

int CDHCP::StartDHCPServer()
{
	pthread_create(&iThreadId,NULL,DHCPServerLauncher,this);
	return 0;
}

void* CDHCP::DHCPServerLauncher(void* aThis)
{
	CDHCP* dhcp = (CDHCP*)aThis;
	dhcp->DHCPThread();
	return NULL;
}

void CDHCP::DHCPThread()
{
	if(!iSocket)
		return;

	if( iSocket->Listen(iDHCPPort) == -1 )
	{
		return;
	}

	int bufferLen = 64;
	char* buffer = new char[bufferLen];

	while(true)
	{
		CDebug::Log("Waiting for client...");

		// wait for client
		iSocket->RecvFrom(buffer,bufferLen);
		CDebug::Log("Received request");

		unsigned short clientPort = ntohs(*((unsigned short*)buffer));

		// select an id
		unsigned short id = GetAvialableClientId();
		if(iDNSServer)
		{
			iDNSServer->UpdateDNS(id,iSocket->GetRemoteAddr(),clientPort);
		}
		else
		{
			CDebug::Error("DNS Server not set");
		}
		// send reply
		unsigned short nwId = htons(id);
		iSocket->SendTo((char*)&nwId,sizeof(unsigned short));
	}
}

unsigned short CDHCP::GetAvialableClientId()
{
	for(int i=0; i<MAX_SUPPORTED_CLIENTS; i++)
	{
		if(!iClientIdAssigned[i])
		{
			iClientIdAssigned[i] = true;
			return i;
		}
	}

	return -1;
}

void CDHCP::FreeClient(unsigned short aClientId)
{
	if(aClientId>=0 && aClientId <MAX_SUPPORTED_CLIENTS)
		iClientIdAssigned[aClientId] = false;

	if(iDNSServer)
		iDNSServer->EraseDNSInfo(aClientId);
}
