/*
 * SDFSMessageQueue.h
 *
 *  Created on: Jan 20, 2011
 *      Author: shashank
 */

#ifndef SDFSMESSAGEQUEUE_H_
#define SDFSMESSAGEQUEUE_H_

#include "SDFSMessage.h"
#include <pthread.h>
#include <queue>
using namespace std;

class SDFSMessageQueue
{
public:
	SDFSMessageQueue();
	~SDFSMessageQueue();

	int InsertMessage(SDFSMessage* aMessage);
	SDFSMessage* RemoveMessage();
	bool IsEmpty();

private:
	bool iIsEmpty;
	pthread_mutex_t iQueueMutex;
	queue<SDFSMessage*> iMessageQueue;

};
#endif /* SDFSMESSAGEQUEUE_H_ */
