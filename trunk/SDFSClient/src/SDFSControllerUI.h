/*
 * SDFSControllerUI.h
 *
 *  Created on: Jan 22, 2011
 *      Author: shashank
 */

#ifndef CONTROLLERUI_H_
#define CONTROLLERUI_H_

class SDFSClient;
#include "SDFSMessage.h";

// TODO: Refactor to pass client's interface
class SDFSControllerUI
{
public:
	SDFSControllerUI(SDFSClient* aClient);
	~SDFSControllerUI();

	void Run();

private: // methods
	void GetMessage();
	void SendMessage(unsigned short aClientId, unsigned long aMessageLength, void* aMessage,TMessageType aMessageType);
	void WriteData();

private:
	SDFSClient* iClient;
	unsigned short iMsgBufferSize;
	char* iMsgBuffer;
};

#endif /* CONTROLLERUI_H_ */
