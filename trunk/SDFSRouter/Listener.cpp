/*
 * Listener.cpp
 *
 *  Created on: Jan 22, 2011
 *      Author: shashank
 */

#include "Listener.h"
#include "SDFSSocket.h"
#include <cstdio>
#include "Router.h"

CListener::CListener(unsigned short aRouterPort, CRouter* aRouter)
//CListener::CListener(unsigned short aRouterPort, queue<CMessage*>* aMessageQueue, pthread_mutex_t* aMutex)
{
	iRouterPort = aRouterPort;
	iSocket = NULL;
	iRouter = aRouter;
	//iQueueMutex = aMutex;
}

CListener::~CListener()
{

}

void CListener::StartListener()
{
	pthread_create(&iThreadId,NULL,ListenerLauncher,this);
}

void* CListener::ListenerLauncher(void* aThis)
{
	CListener* listener = (CListener*)aThis;
	listener->ListenerThread();
}

void CListener::ListenerThread()
{
	if(!iSocket)
	{
		iSocket = new SDFSSocket(SDFSSocket::EUDP);
		iSocket->Listen(iRouterPort);
	}

	int bufferLen = 2048;
	char* buffer = new char[bufferLen];
	while(true)
	{
		printf("waiting for data... ");
		int dataLen = iSocket->RecvFrom(buffer,bufferLen);
		printf("Received Data\n");
		CMessage* msg = new CMessage(dataLen,buffer);
		if(iRouter)
		{
			iRouter->ReceiveMessage(msg);
		}
/*
		pthread_mutex_lock(iQueueMutex);
		printf("Mutex : 0x%x\n",&iQueueMutex);
		printf("Message Queue : 0x%x\n",iMessageQueue);
		iMessageQueue->push(msg);
		pthread_mutex_unlock(iQueueMutex);
*/
	}
}
