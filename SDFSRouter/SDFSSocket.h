/*
 * SDFSSocket.h
 *
 *  Created on: Jan 22, 2011
 *      Author: shashank
 */

#ifndef SDFSSOCKET_H_
#define SDFSSOCKET_H_

#include <sys/socket.h>
#include <sys/types.h>

class SDFSMessage;

/*
 * TSocketState
 *
 *	   +--------------------------------------------+
 *     v											|
 * EInitial ---> EConnected ---> EDisconnected----->+
 *     |											|
 *     +-------> EListening ----------------------->+
 */

class SDFSSocket
{
public:
    enum TSocketType { ETCP, EUDP };

public:
	SDFSSocket(TSocketType aType);
	~SDFSSocket();

	int Connect(const char* aIP, unsigned short aPort);
	int Disconnect();

	int Listen(unsigned short aPort);
	SDFSSocket* Accept();

	int Send(const char* aData, unsigned long aDataLength);
	int Recv(char* aBuffer, unsigned long aDataLenToRecv);

	int SendTo(const char* aData, unsigned long aDataLength);
	int RecvFrom(char* aBuffer, unsigned long aDataLenToRecv);

	bool IsConnected() { return (iSocketState == EConnected); }

	unsigned int GetRemoteAddr();
	unsigned short GetRemotePort();

private: // methods
	void SetupSocket(int aSocket);

private:
	// network data
	int iSocket;
	sockaddr_storage* iRemoteClientAddr;

    static const int BASE_10 = 10;
    static const int MAX_CONNECTION_QUEUE = 5;

    enum TSocketState { EInitial, EDisconnected, EConnected, EListening };

    TSocketState iSocketState;
    TSocketType iSocketType;
};

#endif /* SDFSSOCKET_H_ */
