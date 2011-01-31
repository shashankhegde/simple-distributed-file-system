/*
 * Listener.h
 *
 *  Created on: Jan 22, 2011
 *      Author: shashank
 */

#ifndef LISTENER_H_
#define LISTENER_H_

#include "Message.h"
#include <queue>
#include <pthread.h>

using namespace std;

class SDFSSocket;
class CRouter;

class CListener
{
public:
	//TODO: Refactor
	CListener(unsigned short aRouterPort, CRouter* aRouter);
	//CListener(unsigned short aRouterPort, queue<CMessage*>* aMessageQueue, pthread_mutex_t* aQueueMutex);
	~CListener();

	void StartListener();

private: // methods
	static void* ListenerLauncher(void* aData);
	void ListenerThread();

private: // data
	//queue<CMessage*>* iMessageQueue;
	//pthread_mutex_t* iQueueMutex;
	CRouter* iRouter;

	pthread_t iThreadId;
	unsigned short iRouterPort;
	SDFSSocket* iSocket;

};

#endif /* LISTENER_H_ */
