/*
 * DNS.cpp
 *
 *  Created on: Jan 22, 2011
 *      Author: shashank
 */

#include "DNS.h"
#include "SDFSDebug.h"

CDNS::CDNS(unsigned short aDNSPort)
{
	iDNSPort = aDNSPort;
	iSocket = new SDFSSocket(SDFSSocket::EUDP);
}

CDNS::~CDNS()
{
	delete iSocket;
}

void CDNS::StartDNSServer()
{
	pthread_create(&iThreadId,NULL,DNSServerThreadLauncher,this);
}

void* CDNS::DNSServerThreadLauncher(void* aData)
{
	CDNS* dns = (CDNS*)aData;
	dns->DNSServerThread();

	return NULL;
}

void CDNS::DNSServerThread()
{
	int bufferLen = 32;
	char* buffer = new char[bufferLen];

	if(iSocket->Listen(iDNSPort) == -1)
		return;

	while(true)
	{
		CDebug::Log("DNS: Waiting for client");
		// wait for request
		iSocket->RecvFrom(buffer,bufferLen);

		unsigned short clientId = *((unsigned short*)buffer);

		// lookup
		const TDNSData* dnsData = this->GetDNSData(clientId);

		if(dnsData)
		{
			char data[6];

			char* ptrToIP = data;
			unsigned int* ptrIP = (unsigned int*)ptrToIP;
			*ptrIP = dnsData->iIP;

			char* ptrToPort = data + 4;
			unsigned short* ptrPort = (unsigned short*)ptrToPort;
			*(ptrPort) = dnsData->iPort;
			printf("IP : %x\n", *ptrIP);
			printf("Port : %x\n", *ptrPort);

			// send reply
			iSocket->SendTo(data,6);
		}
	}
}

void CDNS::UpdateDNS(unsigned short aClientId, unsigned int aAddr, unsigned short aPort)
{
	TDNSData* dnsData = new TDNSData;
	dnsData->iClientId = aClientId;
	dnsData->iIP = aAddr;
	dnsData->iPort = aPort;

	printf("DNSUpdate for client %d : %x : %x\n", aClientId, aAddr, aPort);
	iDNSData[aClientId] = dnsData;
}
