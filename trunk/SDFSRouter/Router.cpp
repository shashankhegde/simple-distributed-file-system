/*
 * Router.cpp
 *
 *  Created on: Jan 22, 2011
 *      Author: shashank
 */

#include "Router.h"
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include "SDFSDebug.h"

using namespace std;

CRouter::CRouter(unsigned short aRouterPort, CDNS* aDNSServer, CDHCP* aDHCPServer)
{
	iInQueueMutex = new pthread_mutex_t;
	pthread_mutex_init(iInQueueMutex,NULL);

	printf("Mutex in Router : 0x%x\n",&iInQueueMutex);
	printf("Message Queue in Router: 0x%x\n",&iInMessageQueue);

	iRouterPort = aRouterPort;
	//iListener = new CListener(iRouterPort,&iInMessageQueue,iInQueueMutex);
	iListener = new CListener(iRouterPort,this);
	iDecider = new CDecider(&iInMessageQueue);

	for(int i=0; i<MAX_SUPPORTED_CLIENTS; i++)
	{
		iLinkQuality[i] = 1;
		iLinkSpeed[i] = 1;
	}

	iSendSocket = NULL;
	iDNSServer = aDNSServer;
	iDHCPServer = aDHCPServer;
}

CRouter::~CRouter()
{
	delete iListener;
	delete iDecider;
	pthread_mutex_destroy(iInQueueMutex);
}

void CRouter::RunRouter()
{
	if(iListener)
		iListener->StartListener();

	pthread_create(&iRouterThread,NULL,RouterThreadLauncher,this);
	RouterUI();

}

void* CRouter::RouterThreadLauncher(void* aThis)
{
	CRouter* router = (CRouter*)aThis;
	if(router)
	{
		router->RouterThread();
	}
}

void CRouter::RouterThread()
{
	while(true)
	{
		Route();
		Send();

		sleep(1);
		//usleep(ROUTER_SPEED);
	}
}

void CRouter::RouterUI()
{
	char opt;
	char option[32];
	bool doContinue = true;
	while(doContinue)
	{
		printf("\n\n");
		printf(" Simple Distributed File System\n");
		printf("-------------ROUTER------------\n");
		printf("-------------------------------\n");
		printf("1. Link Status\n");
		printf("2. Modify Link Status\n");
		printf("3. Drop Packets\n");
		printf("4. Kick Client\n");
		printf("0. Exit\n\n");
		printf("-> ");

		opt = getchar();
		printf("option : %d\n", opt);
		//opt = cin.get();
		switch(opt)
		{
		case '1':
			LinkStatus();
			break;
		case '2':
			ModifyLinkStatus();
			break;
		case '3':
			DropPackets();
			break;
		case '4':
			KickClient();
			break;
		case '0':
			doContinue = false;
			break;
		default:
			CDebug::Log("Unknown option");
		}

		getchar();
	}

}

void CRouter::LinkStatus()
{
	printf("\n\n");
	printf("Link Status\n");
	for(int i=0; i<MAX_SUPPORTED_CLIENTS;i++)
	{
		printf("Link [%d] ",i+1);
		printf("%d : %f : %d\n", iLinkQuality[i], iLinkSpeed[i], iPerClientMessageQueue[i].size());
	}
}

void CRouter::ModifyLinkStatus()
{
	int linkId;
	printf("Link id : ");
	scanf("%d",&linkId);
	printf("Modifying link %d\n",linkId);

	if(linkId < MAX_SUPPORTED_CLIENTS)
	{
		printf("Enter link speed: ");
		int speed;
		scanf("%d",&speed);
		iLinkSpeed[linkId-1] = speed;
	}
	else
	{
		printf("%d does not exist ", linkId);
	}
}

void CRouter::DropPackets(int id)
{
	int linkId;
	if(id == -1)
	{
		printf("Link id : ");
		scanf("%d",&linkId);
	}
	else
		linkId = id;

	if(linkId < MAX_SUPPORTED_CLIENTS)
	{
		while(!iPerClientMessageQueue[linkId-1].empty())
		{
			CMessage* msg = iPerClientMessageQueue[linkId-1].front();
			delete msg;
			iPerClientMessageQueue[linkId-1].pop();
		}

		printf("Dropped packets from %d\n",linkId);
	}
	else
	{
		printf("%d does not exist ", linkId);
	}
}

void CRouter::KickClient()
{
	int linkId;
	printf("Link id : ");
	scanf("%d",&linkId);

	if(linkId < MAX_SUPPORTED_CLIENTS)
	{
		DropPackets(linkId-1);
		iLinkQuality[linkId-1] = 1;
		iLinkSpeed[linkId-1] = 1;
		iDHCPServer->FreeClient(linkId-1);
	}
	else
	{
		printf("%d does not exist ", linkId);
	}
}

void CRouter::Route()
{
	while(true)
	{
		pthread_mutex_lock(iInQueueMutex);
		if(iInMessageQueue.empty())
		{
			pthread_mutex_unlock(iInQueueMutex);
			break;
		}
		CMessage* msg = iInMessageQueue.front();
		iInMessageQueue.pop();
		pthread_mutex_unlock(iInQueueMutex);

		unsigned short clientId = msg->GetDestinationId();
		if(clientId < MAX_SUPPORTED_CLIENTS)
		{
			iPerClientMessageQueue[clientId].push(msg);
		}
	}
}

void CRouter::Send()
{
	if(iSendSocket == NULL)
	{
		iSendSocket = new SDFSSocket(SDFSSocket::EUDP);
		if(!iSendSocket)
			return;
	}

	for(int i=0; i<MAX_SUPPORTED_CLIENTS; i++)
	{
		if(!iPerClientMessageQueue[i].empty())
		{
			int sendCount = 0;
			// min
			int limit = (iPerClientMessageQueue[i].size() > iLinkSpeed[i]) ? iLinkSpeed[i] : iPerClientMessageQueue[i].size();
			while(limit)
			{
				CMessage* msg = iPerClientMessageQueue[i].front();
				iPerClientMessageQueue[i].pop();
				limit = (iPerClientMessageQueue[i].size() > iLinkSpeed[i]) ? iLinkSpeed[i] : iPerClientMessageQueue[i].size();

				// send message to the client
				if(msg)
				{
					if(iDNSServer)
					{
						const TDNSData* dns = iDNSServer->GetDNSData(msg->GetDestinationId());
						if(dns)
						{
							unsigned int i = dns->iIP;
							unsigned char* cp = (unsigned char*)&i;
							char ip[24] = {0};
							sprintf(ip,"%d.%d.%d.%d",cp[3],cp[2],cp[1],cp[0]);
							printf("Sending to %x, %s : %d...\n",i, ip,dns->iPort);
							iSendSocket->Connect(ip,dns->iPort);
							iSendSocket->SendTo(msg->GetData(),msg->GetLength());
							iSendSocket->Disconnect();
						}
					}
				}
/*
				delete msg;
				msg = NULL;
*/
			}
		}
	}
}

void CRouter::ReceiveMessage(CMessage* aMessage)
{
	pthread_mutex_lock(iInQueueMutex);
	iInMessageQueue.push(aMessage);
	pthread_mutex_unlock(iInQueueMutex);
}
