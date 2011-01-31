/*
 * Decider.h
 *
 *  Created on: Jan 22, 2011
 *      Author: shashank
 */

#ifndef DECIDER_H_
#define DECIDER_H_

#include "Message.h"
#include "Constants.h"

#include <queue>
using namespace std;

class CDecider
{
public:
	CDecider(queue<CMessage*>* aMessageQueue);
	~CDecider();

	void RunDecider();
	CMessage* GetMessage();

	void ReOrderMessage();

private: // methods
	void ReOrderMessages();
	void DropMessages();

private: // data
	queue<CMessage*>* iMessageQueue;
	queue<CMessage*>  iPerClientMessageQueue[MAX_SUPPORTED_CLIENTS]; // array of queues! Lets keep space for 10 clients
};

#endif /* DECIDER_H_ */
