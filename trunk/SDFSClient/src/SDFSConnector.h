/*
 * SDFSConnector.h
 *
 *  Created on: Jan 13, 2011
 *      Author: shashank
 */

#ifndef SDSCONNECTOR_H_
#define SDSCONNECTOR_H_

#include "SDFSSocket.h"
#include "SDFSMessage.h"
#include "DNSClient.h"

class SDFSConnector
{
public:
	SDFSConnector(unsigned short aRemoteClientId, char* aRouterIp);//, char* aRemoteIp, unsigned short aRemotePort);
	~SDFSConnector();

	int Send(const SDFSMessage* aMessage);
	SDFSMessage* Recv();

	unsigned short GetRemoteClientId() { return iRemoteClientId; }

private:
	unsigned short iRemoteClientId;

	SDFSSocket* iSocket;
	CDNSClient* iDNSClient;
	char* iRouterIp;
};
#endif /* SDFSCONNECTOR_H_ */
