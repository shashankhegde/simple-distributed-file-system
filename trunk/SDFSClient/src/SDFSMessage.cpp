/*
 * SDFSMessage.cpp
 *
 *  Created on: Jan 14, 2011
 *      Author: shashank
 */

#include "SDFSMessage.h"

#ifndef NULL
#define NULL 0
#endif

SDFSMessage::SDFSMessage()
{
	SetMessageData(NULL,0);
}

SDFSMessage::SDFSMessage(const char* aData, unsigned long aDataLength)
{
	SetMessageData(aData,aDataLength);
}

SDFSMessage::~SDFSMessage()
{
	if(iData)
		delete[] iData;
}

void SDFSMessage::SetMessageId(unsigned long aMessageId)
{
	iMessageId = aMessageId;
}

int SDFSMessage::SetMessageData(const char* aData, unsigned long aDataLength)
{
	iData = aData;
	iDataLength = aDataLength;

	return 0;
}

unsigned long SDFSMessage::GetMessageId() const
{
	return iMessageId;
}

unsigned long SDFSMessage::GetDataLength() const
{
	return iDataLength;
}

const char* SDFSMessage::GetMessageData() const
{
	return iData;
}
