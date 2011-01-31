/*
 * SDFSSocket.cpp
 *
 *  Created on: Jan 22, 2011
 *      Author: shashank
 */

#include "SDFSSocket.h"
#include "SDFSDebug.h"

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <sstream>

using namespace std;

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

SDFSSocket::SDFSSocket(TSocketType aSocketType)
{
	iSocket = -1;

	iSocketState = EInitial;
	iSocketType = aSocketType;
	iRemoteClientAddr = new sockaddr_storage;

}

SDFSSocket::~SDFSSocket()
{
	Disconnect();
}

void SDFSSocket::SetupSocket(int aSocket)
{
	iSocket = aSocket;
	iSocketState = EListening;
}

int SDFSSocket::Connect(const char* aIP, unsigned short aPort)
{
	if((iSocketState != EInitial) && (iSocketState != EDisconnected))
	{
		CDebug::Error("Not in initial state!");
		return -1;
	}

/*
 * Redundant check
 *
	if(iSocket != -1)
	{
		// we are already connected
		CDebug::Log("CNetworkStack::Connect(), already connected");
	}
*/

	char portAsCString[10] = {0};
	sprintf(portAsCString,"%d",aPort);
/*
	ostringstream portAsString;
	portAsString << aPort;
	const char* portAsCString = (portAsString.str()).c_str();
*/

	struct addrinfo* addrResults;
	struct addrinfo addrHints;
	memset(&addrHints,NULL,sizeof(addrinfo));
	addrHints.ai_family = AF_UNSPEC;
	if(iSocketType == ETCP)
		addrHints.ai_socktype = SOCK_STREAM;
	else if(iSocketType == EUDP)
		addrHints.ai_socktype = SOCK_DGRAM;

	int error = getaddrinfo(aIP,portAsCString,&addrHints,&addrResults);
	if(error != 0)
	{
		CDebug::Error("Error getting addr info");
		return -1;
	}

	// find the first addr
	struct addrinfo* iter = addrResults;
	for(iter=addrResults;iter!=NULL;iter=iter->ai_next)
	{
		iSocket = socket(iter->ai_family,iter->ai_socktype, iter->ai_protocol);
		if(iSocket == -1)
		{
			CDebug::Log("Error creating a socket, trying next");
			continue;
		}
		else
		{
			error = connect(iSocket, iter->ai_addr, iter->ai_addrlen);
			if(error != 0)
			{
				CDebug::Error("Could not connect");
			}
			else
			{
				if(iSocketType == SDFSSocket::EUDP)
					iRemoteClientAddr = (sockaddr_storage*)(addrResults->ai_addr);
				CDebug::Log("Connected!");
				break;
			}
		}
	}

	if(iSocketType == SDFSSocket::ETCP)
		freeaddrinfo(addrResults);

	if(iter == NULL)
	{
		CDebug::Error("connect failed");
		return -1;
	}

	iSocketState = EConnected;
	CDebug::Log("Connection successfully established!");
	return 0;
}

int SDFSSocket::Disconnect()
{
	if(iSocketState != EConnected)
	{
		CDebug::Error("CNetworkStack::Disconnect(), not connected!");
		return 0; // TODO: Should I really succeed?
	}

	close(iSocket);
	iSocket = -1;
	iSocketState = EDisconnected;
	return 0;
}

int SDFSSocket::Listen(unsigned short aPort)
{
	if(iSocketState != EInitial)
	{
		CDebug::Error("Listen(), not in initial state!");
		return -1;
	}

	struct addrinfo addrHints;
	memset(&addrHints,NULL,sizeof(addrinfo));
	addrHints.ai_family = AF_UNSPEC;
	if(iSocketType == ETCP)
		addrHints.ai_socktype = SOCK_STREAM;
	else if(iSocketType == EUDP)
		addrHints.ai_socktype = SOCK_DGRAM;
	addrHints.ai_flags = AI_PASSIVE;

	ostringstream portAsString;
	portAsString << aPort;
	const char* portAsCString = (portAsString.str()).c_str();

	struct addrinfo* serverInfo;
	int error = getaddrinfo(NULL,portAsCString,&addrHints,&serverInfo);
	if(error!=0)
	{
		CDebug::Error("Listen(), getaddrinfo");
		return error;
	}

	struct addrinfo* iter = serverInfo;
	for(iter=serverInfo;iter!=NULL;iter=iter->ai_next)
	{
		iSocket = socket(iter->ai_family,iter->ai_socktype,iter->ai_protocol);
		if(iSocket==-1)
		{
			CDebug::Error("Socket()");
			continue;
		}

		int yes=1;
		socklen_t size = sizeof(int);
		error = getsockopt(iSocket,SOL_SOCKET,SO_REUSEADDR,&yes,&size);
		if(error == -1)
		{
			CDebug::Error("getsocketopt()");
			return -1;
		}

		error = bind(iSocket,iter->ai_addr,iter->ai_addrlen);
		if(error == -1)
		{
			perror("bind");
			close(iSocket);
			continue;
		}
		else
		{
			break;
		}
	}

	freeaddrinfo(serverInfo);

	if(iter==NULL)
	{
		CDebug::Error("Couldn't bind");
		return -1;
	}

	if(iSocketType == SDFSSocket::ETCP)
	{
		// setup listening queue
		error = listen(iSocket, MAX_CONNECTION_QUEUE);
		if(error == -1)
		{
			CDebug::Error("listen()");
			return -1;
		}
	}

	iSocketState = EListening;

	return 0;
}

SDFSSocket* SDFSSocket::Accept()
{
	if(iSocketState != EListening)
	{
		CDebug::Error("Not in Listening state");
		return NULL;
	}

	int s = accept(iSocket,NULL,0);
	SDFSSocket* newSocket = new SDFSSocket(SDFSSocket::ETCP);

	if(newSocket)
		newSocket->SetupSocket(s);

	return newSocket;
}

int SDFSSocket::Send(const char* aData, unsigned long aDataLen)
{
	if(iSocketState != EConnected)
	{
		CDebug::Error("Cannot Send() without connecting!");
		return -1;
	}

	int dataToSend = aDataLen;
	const char* dataPtr = aData;

	while(dataToSend>0)
	{
		int sentData = send(iSocket,dataPtr,dataToSend,0);
		if(sentData < 0)
		{
			CDebug::Error("Can't send data");
			return -1;
		}
		dataToSend -= sentData;
		dataPtr += sentData;
	}

	return 0;
}

int SDFSSocket::Recv(char* aBuffer, unsigned long aDataRecvLen)
{
	if(iSocketState != EConnected)
	{
		CDebug::Error("Recv(), but not connected!");
		return -1;
	}

	int dataToRecv = aDataRecvLen;
	char* dataPtr = aBuffer;

	while(dataToRecv>0)
	{
		int recvdData = recv(iSocket,dataPtr,dataToRecv,0);
		if(recvdData < 0)
		{
			CDebug::Error("Can't recv data");
			return -1;
		}
		dataToRecv -= recvdData;
		dataPtr += recvdData;
	}

	return 0;
}

int SDFSSocket::SendTo(const char* aData, unsigned long aDataLen)
{

/*
	if(iSocketState != EConnected)
	{
		CDebug::Error("Cannot Send() without connecting!");
		return -1;
	}
*/

/*
	printf("Sending data : ");
	for(int i=0; i<aDataLen; i++)
		printf("%d ", (char)aData[i]);
	printf("\n");
*/

	socklen_t addrLen = sizeof(sockaddr_storage);
	int sentData = sendto(iSocket,aData,aDataLen,0,(sockaddr*)iRemoteClientAddr,addrLen);
	if(sentData < 0)
	{
		CDebug::Error("Can't send data");
		return -1;
	}

	return 0;
}

int SDFSSocket::RecvFrom(char* aBuffer, unsigned long aDataRecvLen)
{
/*
	if(iSocketState != EConnected)
	{
		CDebug::Error("Recv(), but not connected!");
		return -1;
	}
*/

	socklen_t addrLen = sizeof(sockaddr_storage);
	int recvdData = recvfrom(iSocket,aBuffer,aDataRecvLen,0,(sockaddr*)iRemoteClientAddr,&addrLen);
	if(recvdData < 0)
	{
		CDebug::Error("Can't recv data");
		return -1;
	}

	return recvdData;
}

unsigned int SDFSSocket::GetRemoteAddr()
{
	if(iRemoteClientAddr)
	{
		unsigned int addr = ((sockaddr_in*)iRemoteClientAddr)->sin_addr.s_addr;
		return ntohl(addr);
	}

	return 0;
}

unsigned short SDFSSocket::GetRemotePort()
{
	if(iRemoteClientAddr)
	{
		return ((sockaddr_in*)iRemoteClientAddr)->sin_port;
	}

	return 0;
}
