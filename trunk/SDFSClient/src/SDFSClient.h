/*
 * SDFSClient.h
 *
 *  Created on: Jan 20, 2011
 *      Author: shashank
 */

#ifndef SDFSCLIENT_H_
#define SDFSCLIENT_H_

#include "SDFSMessage.h"
#include "SDFSMessageQueue.h"
#include "SDFSControllerUI.h"
#include "SDFSConnector.h"
#include "DHCPClient.h"
#include "logger/Dictionary.h"
#include "logger/EventClock.h"

#include <pthread.h>
#include <list>

class SDFSListener;

using namespace std;

class SDFSClient /*: public MClientConnector*/ //-> Interface to queues and to add connector to the client
{
public:
	SDFSClient(unsigned short aClientId, unsigned short aListenerPort, char* aRouterIp);
	~SDFSClient();

	unsigned short GetId() { return iClientId; }
	void RunClient();
	void SendMessage(SDFSMessage* aMessage);	// locally generated messages
	void ReceivedMessage(SDFSMessage* aMessage);
	int  GetTime() { return iClock.GetTime(); }

	void ShowTimeTable();
	void Lookup(string& aKey);
	void ListKeys();

private:
	static void* ClientThreadLauncher(void*);
	void ClientThread();
	void MessageProcessor(SDFSMessage* aMessage);
	void PrintMessage(SDFSMessage* aMessage);
	void ProcessData(SDFSMessage* aMessage);
	void AppendData(SDFSMessage* aMessage);
	void ProcessDictionaryEvent(SDFSMessage* aMessage);
	void ProcessTimeTable(SDFSMessage* aMessage);
	void ProcessLog(SDFSMessage* aMessage);

	static void* SenderThreadLauncher(void*);
	void SenderThread();
	void ProcessMessage(SDFSMessage* aMessage);
	SDFSConnector* GetConnector(unsigned short aRemoteHostId);

	void Sync(unsigned short aDestination);

private:
	unsigned short iClientId;
	unsigned short iListenerPort;
	SDFSListener* iListener;
	SDFSControllerUI* iUI;

	SDFSMessageQueue* iInMessageQueue;
	SDFSMessageQueue* iOutMessageQueue;

	list<SDFSConnector*> iConnectors;

	pthread_t iClientThreadId;
	pthread_t iSenderThreadId;

	CDHCPClient* iDHCPClient;
	char* iRouterIp;

	Dictionary* iDictionary;
	TimeTable* iTimeTable;
	EventClock iClock;
};

#endif /* SDFSCLIENT_H_ */
