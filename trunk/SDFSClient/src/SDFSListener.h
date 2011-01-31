/*
 * SDFSListener.h
 *
 *  Created on: Jan 15, 2011
 *      Author: shashank
 */

#ifndef SDFSLISTENER_H_
#define SDFSLISTENER_H_

#include <pthread.h>

#include "SDFSSocket.h"
#include "SDFSClient.h"

class SDFSClient;

class SDFSListener
{
public:
	static SDFSListener* Instance(SDFSClient* aClient, unsigned short aPort);	// singleton object (required?)
	~SDFSListener();

	int Start();
	int Stop();

private: // methods
	SDFSListener(SDFSClient* aClient, unsigned short aPort);

	int HandleConnection(SDFSSocket* aSocket);
	static void* ConnectorSpawner(SDFSSocket* aSocket);
	static void* ListenerThreadSpawner(void* aGFSListener);
	void* ListenerThread(void* aData);

	void ReceiveMessage();


private: // data
	SDFSClient* iClient; // not owned
	int iPort;
	pthread_t iListenerThread;
	static SDFSListener* iInstance;

	bool iListening;

	SDFSSocket* iSocket;
};

#endif /* SDFSLISTENER_H_ */
