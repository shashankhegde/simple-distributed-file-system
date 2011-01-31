/*
 * DHCPClient.h
 *
 *  Created on: Jan 23, 2011
 *      Author: shashank
 */

#ifndef DHCPCLIENT_H_
#define DHCPCLIENT_H_

#include "SDFSSocket.h"

class CDHCPClient
{
public:
	CDHCPClient(char* iDHCPServerIp);
	~CDHCPClient();

	unsigned short GetClientId(unsigned short aSDFSClientPort);

private:
	SDFSSocket* iSocket;

	char* iDHCPServerIp;
	unsigned short iDHCPPort;
};

#endif /* DHCPCLIENT_H_ */
