/*
 * DNS.h
 *
 *  Created on: Jan 22, 2011
 *      Author: shashank
 */

#ifndef DNS_H_
#define DNS_H_

#include <pthread.h>
#include <cstring>
#include <map>
using namespace std;

#include "SDFSSocket.h"
#include <sys/types.h>

class TDNSData
{
public:
	TDNSData()
	{
		iClientId = 0;
		iPort = 0;
		iIP = 127;
		iIP <<= 8*2;
		iIP |= 1;
	}

	unsigned short iClientId;
	unsigned int iIP;
	unsigned short iPort;
};

class CDNS
{
public:
	CDNS(unsigned short aDNSPort);
	~CDNS();

	void StartDNSServer();
	void UpdateDNS(unsigned short aClientId, unsigned int aIP, unsigned short aPort);
	const TDNSData* GetDNSData(unsigned short aClientId)
	{
		TDNSData* dnsData = NULL;
		if(aClientId >= 0)
		{
			dnsData = iDNSData[aClientId]; //.at(aClientId);
		}

		return dnsData;
	}

	void EraseDNSInfo(unsigned short aClientId)
	{
		iDNSData.erase(aClientId);
	}


private:
	static void* DNSServerThreadLauncher(void* aData);
	void DNSServerThread();


private:
	TDNSData iDefaultData;
	map<int,TDNSData*> iDNSData;
	unsigned short iDNSPort;
	pthread_t iThreadId;

	SDFSSocket* iSocket;

};
#endif /* DNS_H_ */
