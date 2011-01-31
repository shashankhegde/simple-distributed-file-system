/*
 * DHCP.h
 *
 *  Created on: Jan 22, 2011
 *      Author: shashank
 */

#ifndef DHCP_H_
#define DHCP_H_

#include <pthread.h>
#include "Constants.h"
#include "SDFSSocket.h"
#include "DNS.h"

class CDHCP
{
public:
	CDHCP(unsigned short aDHCPPort, CDNS* aDNSServer);
	~CDHCP();

	int StartDHCPServer();
	void FreeClient(unsigned short aClientId);

private:
	static void* DHCPServerLauncher(void* aData);
	void DHCPThread();
	unsigned short GetAvialableClientId();

private: // data
	bool iClientIdAssigned[MAX_SUPPORTED_CLIENTS];
	unsigned short iDHCPPort;

	pthread_t iThreadId;
	SDFSSocket* iSocket;
	CDNS* iDNSServer;
};
#endif /* DHCP_H_ */
