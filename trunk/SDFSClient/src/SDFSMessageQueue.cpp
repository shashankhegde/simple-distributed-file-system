/*
 * SDFSMessageQueue.cpp
 *
 *  Created on: Jan 20, 2011
 *      Author: shashank
 */

#include "SDFSMessageQueue.h"
#include "SDFSDebug.h"

SDFSMessageQueue::SDFSMessageQueue()
{
	iIsEmpty = true;
	pthread_mutex_init(&iQueueMutex,NULL);
}

SDFSMessageQueue::~SDFSMessageQueue()
{
	pthread_mutex_destroy(&iQueueMutex);
}

int SDFSMessageQueue::InsertMessage(SDFSMessage* aMessage)
{
	// begin critical section
	pthread_mutex_lock(&iQueueMutex);

	// insert message into the queue
	iMessageQueue.push(aMessage);

	// end critical section
	pthread_mutex_unlock(&iQueueMutex);

	return 0;
}

SDFSMessage* SDFSMessageQueue::RemoveMessage()
{
	SDFSMessage* msg = NULL;

	// begin critical section
	pthread_mutex_lock(&iQueueMutex);

	// remove message from the queue

	if(!iMessageQueue.empty())
	{
		msg = iMessageQueue.front();
		iMessageQueue.pop();
	}

	// end critical section
	pthread_mutex_unlock(&iQueueMutex);

	return msg;
}

bool SDFSMessageQueue::IsEmpty()
{
	bool isEmpty = true;

	// begin critical section
	pthread_mutex_lock(&iQueueMutex);

	isEmpty = iMessageQueue.empty();

	// end critical section
	pthread_mutex_unlock(&iQueueMutex);

/*
	if(isEmpty)
		CDebug::Log("Empty!");
	else
		CDebug::Log("Not Empty!");
*/
	return isEmpty;
}
