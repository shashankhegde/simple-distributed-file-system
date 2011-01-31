/*
 * Message.h
 *
 *  Created on: Jan 22, 2011
 *      Author: shashank
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "Utils.h"

#include <cstdio>
using namespace std;

class CMessage
{
public:
	CMessage(int aLength, char* aData)
	{
		iData = aData;
		iLength = aLength;
	}
	~CMessage()
	{
		delete[] iData;
	}

	int GetLength() { return iLength; }
	char* GetData() { return iData; }
	unsigned short GetDestinationId()
	{
		// first 2 bytes of data contain the destination client id
		unsigned short id = UnpackShort((unsigned char*)iData);//*((unsigned short*)iData);
		printf("Message destined for %d\n",id);
		return id;
	}

private:
	int iLength;
	char* iData;
};

#endif /* MESSAGE_H_ */
