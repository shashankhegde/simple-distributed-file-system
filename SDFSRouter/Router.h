/*
 * Router.h
 *
 *  Created on: Jan 22, 2011
 *      Author: shashank
 */

#ifndef ROUTER_H_
#define ROUTER_H_

#include "Listener.h"
#include "Decider.h"
#include "Message.h"
#include <pthread.h>

#include <queue>

using namespace std;

#include "Constants.h"
#include "DNS.h"
#include "DHCP.h"
#include "SDFSSocket.h"

// Loop delay in ms
#define ROUTER_SPEED 100000

class CRouter
{
public:
	CRouter(unsigned short aRouterPort, CDNS* aDNSServer, CDHCP* aDHCPServer);
	~CRouter();

	void RunRouter();
	void ReceiveMessage(CMessage* aMessage);

private: // methods
	static void* RouterThreadLauncher(void* aThis);
	void RouterThread();

	void RouterUI();

	void Route();
	void Send();

	void LinkStatus();
	void ModifyLinkStatus();
	void DropPackets(int aId=-1);
	void KickClient();

private: // data
	unsigned short iRouterPort;
	CListener* iListener;
	queue<CMessage*> iInMessageQueue; // owned
	CDecider* iDecider;

	int iLinkSpeed[MAX_SUPPORTED_CLIENTS]; // number of messages sent in one cycle
	double iLinkQuality[MAX_SUPPORTED_CLIENTS]; // measure of link quality. 0 - down, 1 - lossless, inbetween - lossy
	queue<CMessage*> iPerClientMessageQueue[MAX_SUPPORTED_CLIENTS]; // array of queues!

	pthread_mutex_t* iInQueueMutex;
	CDNS* iDNSServer;
	CDHCP* iDHCPServer;
	SDFSSocket* iSendSocket;

	pthread_t iRouterThread;
};

#endif /* ROUTER_H_ */
