/*
 * SDFSClient.cpp
 *
 *  Created on: Jan 21, 2011
 *      Author: shashank
 */

#include "SDFSClient.h"
#include "SDFSDebug.h"
#include "SDFSListener.h"
#include <fstream>
#include <cstring>

using namespace std;

SDFSClient::SDFSClient(unsigned short aClientId, unsigned short aListenerPort, char* aRouterIp)
{
	iRouterIp = new char[strlen(aRouterIp)+1];
	strcpy(iRouterIp,aRouterIp);

	iDHCPClient = new CDHCPClient(iRouterIp);
	if(iDHCPClient)
	{
		iClientId = iDHCPClient->GetClientId(aListenerPort);
	}

	iInMessageQueue = NULL;
	iOutMessageQueue = NULL;
	iListener = NULL;
	iListenerPort = aListenerPort;
	iUI = new SDFSControllerUI(this);

	if(!iInMessageQueue)
	{
		iInMessageQueue = new SDFSMessageQueue();
		if(!iInMessageQueue)
			return;
	}

	if(iOutMessageQueue == NULL)
	{
		iOutMessageQueue = new SDFSMessageQueue();
		if(!iOutMessageQueue)
			return;
	}

	CDebug::SetClientId(iClientId);
}

SDFSClient::~SDFSClient()
{
	delete iInMessageQueue;
	delete iOutMessageQueue;
}

void SDFSClient::RunClient()
{
	int error = 0;

	// Start ClientThread
	error = pthread_create(&iClientThreadId,NULL,ClientThreadLauncher,this);
	if(error != 0)
	{
		CDebug::Error("Unable to create ClientThread");
		return;
	}

	// Start SenderThread
	error = pthread_create(&iSenderThreadId,NULL,SenderThreadLauncher,this);
	if(error != 0)
	{
		CDebug::Error("Unable to create SenderThread");
		return;
	}

	// Start Listener
	if(iListener)
	{
		delete iListener;
		iListener = NULL;
	}
	iListener = SDFSListener::Instance(this,iListenerPort);
	iListener->Start();

	// Fireup UI to control message transfers
	if(!iUI)
	{
		CDebug::Error("No UI!");
		return;
	}

	iUI->Run();
}

void* SDFSClient::ClientThreadLauncher(void* aData)
{
	SDFSClient* thisClient = (SDFSClient*)aData;
	if(aData == NULL)
		return NULL;

	thisClient->ClientThread();
	return NULL;
}

void* SDFSClient::SenderThreadLauncher(void* aData)
{
	SDFSClient* thisClient = (SDFSClient*)aData;
	if(aData == NULL)
		return NULL;

	thisClient->SenderThread();
	return NULL;
}

void SDFSClient::SendMessage(SDFSMessage* aMessage)
{
	CDebug::Log("Inserting message into out queue");
	iOutMessageQueue->InsertMessage(aMessage);
}

void SDFSClient::ReceivedMessage(SDFSMessage* aMessage)
{
	CDebug::Log("Received a message!");
	iInMessageQueue->InsertMessage(aMessage);
}

void SDFSClient::ClientThread()
{
	while(true)
	{
		if(!iInMessageQueue->IsEmpty())
		{
			SDFSMessage* msg = iInMessageQueue->RemoveMessage();
			if(msg)
				MessageProcessor(msg);
			delete msg;
		}

		usleep(100*1000);
	}
}

void SDFSClient::SenderThread()
{
	while(true)
	{
		if(iOutMessageQueue)
		{
			if(!iOutMessageQueue->IsEmpty())
			{
				cout << "Sending Message\n";
				SDFSMessage* msg = iOutMessageQueue->RemoveMessage();

				SDFSConnector* connector = GetConnector(msg->GetDestination());
				if(connector)
					connector->Send(msg);

				delete msg;
				msg = NULL;
/*
				list<SDFSConnector*>::iterator iter;
				for(iter = iConnectors.begin(); iter != iConnectors.end(); iter++)
				{
					SDFSConnector* connector = *iter;
					if(connector && connector->GetRemoteClientId() == msg->GetDestination())
					{
						// use this connector to send data
						connector->Send(msg);
					}
				}
*/
			}
		}

		usleep(100*1000);
	}
}

void SDFSClient::MessageProcessor(SDFSMessage* aMessage)
{
	if(aMessage == NULL)
	{
		// not a valid message
		CDebug::Log("NULL message!");
		return;
	}

	// based on the message type, take an action
	switch(aMessage->GetMessageType())
	{
	case EString:
		PrintMessage(aMessage);
		break;
	case EData:
		ProcessMessage(aMessage);
		break;
	case EAppendData:
		AppendData(aMessage);
		break;
	default:
		CDebug::Log("Unknown message!");
	}

}

void SDFSClient::AppendData(SDFSMessage* aMessage)
{
	if(iClientId != 0)
	{
		cout << "Client id is not 0, hence not the file server\n";
		return;
	}
	if(!aMessage)
	{
		cout << "NULL message\n";
		return;
	}

	ofstream fout("SDFS.data",ios::app | ios::binary);
	cout << "Appending data from client " << aMessage->GetSource() << endl;
	fout.write(aMessage->GetMessageData(),aMessage->GetDataLength());
	fout.close();
}

void SDFSClient::PrintMessage(SDFSMessage* aMessage)
{
	if(aMessage == NULL)
	{
		// not a valid message
		CDebug::Log("NULL message!");
		return;
	}

	cout << "Message received -> ";
	// data part of the message is a null terminated string
	cout << aMessage->GetSource() << " : " << aMessage->GetMessageId() << " : " \
		 << aMessage->GetDataLength() << " : " << aMessage->GetMessageData() << endl;
}

void SDFSClient::ProcessMessage(SDFSMessage* aMessage)
{
	CDebug::Log("Not yet implemented");
}

SDFSConnector* SDFSClient::GetConnector(unsigned short aRemoteClientId)
{
	list<SDFSConnector*>::iterator iter;
	for(iter = iConnectors.begin(); iter != iConnectors.end(); iter++)
	{
		SDFSConnector* connector = *iter;
		if(connector && connector->GetRemoteClientId() == aRemoteClientId)
		{
			// use this connector to send data
			return connector;
		}
	}

	// Did not find a connector, so create one
	SDFSConnector* newConnector = new SDFSConnector(aRemoteClientId,iRouterIp);
	iConnectors.push_front(newConnector);	// latest is at the front of the queue
	return newConnector;
}
