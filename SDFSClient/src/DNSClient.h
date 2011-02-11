/*
 * DNSClient.h
 *
 *  Created on: Jan 22, 2011
 *      Author: shashank
 */

#ifndef DNSCLIENT_H_
#define DNSCLIENT_H_

#include "SDFSSocket.h"

#ifndef NULL
#define NULL
#endif

struct TDNSInfo
{
	// aaa.bbb.ccc.ddd
	char iIP[16];
	unsigned short iPort;
};

class CDNSClient
{
public:
	static CDNSClient* Instance(char* aDNSServerIp); // singleton
	static void Delete();

	TDNSInfo* GetIPAddr(unsigned short aClientId);

protected:
	CDNSClient(char* aDNSServerIp);
	~CDNSClient();

private: // data
	static CDNSClient* iDNSClient;
	SDFSSocket* iSocket;

	char* iDNSServerIp;
	unsigned short iDNSPort;
	static int iInstanceCount;
};


#endif /* DNSCLIENT_H_ */
