/*
 * SDFSMessage.h
 *
 *  Created on: Jan 14, 2011
 *      Author: shashank
 */

#ifndef SDFSMESSAGE_H_
#define SDFSMESSAGE_H_


/*
 *   |--------------------------------------------------------------------------------------|
 *   | DestClientId (2) | SrcClientId (2) | MsgType (2) | MsgId (4) | DataLen (4) | MsgData |
 *   |--------------------------------------------------------------------------------------|
 */


enum TMessageType { EString, EData, EAppendData, ERequestData, EResendData, ELog, ETimeTable, EDictionaryEvent };
// missing data is requested by sending a ERequestData message, which specifies byte range
// In response, the client sends data through a EResendMessage which contains byte range of the data

class SDFSMessage
{
public:
	SDFSMessage();
	SDFSMessage(const char* aData, unsigned long aDataLength);
	~SDFSMessage();

	void SetDestination(unsigned short aDestination) { iDestinationId = aDestination; }
	void SetSource(unsigned short aSource) { iSourceId = aSource; }
	void SetMessageId(unsigned long aMessageId);
	int  SetMessageData(const char* aData, unsigned long aDataLength);
	void SetMessageType(TMessageType aMessageType) { iMessageType = aMessageType; }

	unsigned long GetMessageId() const;
	const char* GetMessageData() const;
	unsigned long GetDataLength() const;
	unsigned short GetMessageType() const { return (unsigned short)iMessageType; }
	unsigned short GetSource() const { return iSourceId; }
	unsigned short GetDestination() const { return iDestinationId; }

private:
	unsigned long iMessageId;
	const char* iData; // ownership is transferred
	unsigned long iDataLength;
	TMessageType iMessageType;
	unsigned short iSourceId;
	unsigned short iDestinationId;
};

#endif /* SDFSMESSAGE_H_ */
