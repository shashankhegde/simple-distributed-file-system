/*
 * Decider.cpp
 *
 *  Created on: Jan 22, 2011
 *      Author: shashank
 */

#include "Decider.h"

CDecider::CDecider(queue<CMessage*>* aMessageQueue)
{
	iMessageQueue = aMessageQueue;
}

CDecider::~CDecider()
{
	iMessageQueue = NULL;
}

void CDecider::RunDecider()
{
	// extract CMessages from common queue and insert into per client queue

	while(!iMessageQueue->empty())
	{
		CMessage* msg = iMessageQueue->back();
		iMessageQueue->pop();

		unsigned short clientId = msg->GetDestinationId();
		if(clientId < MAX_SUPPORTED_CLIENTS)
		{
			iPerClientMessageQueue[clientId].push(msg);
		}
	}

}
