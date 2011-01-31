/*
 * Utils.cpp
 *
 *  Created on: Jan 25, 2011
 *      Author: shashank
 */

void PackShort(unsigned short aVal, unsigned char aBuffer[2])
{
	unsigned short mask = 0xff00;
	unsigned char byte;
	byte = ((aVal&mask)>>8);
	aBuffer[0] = byte;

	mask >>= 8;

	byte = ((aVal&mask));
	aBuffer[1] = byte;

}

unsigned short UnpackShort(unsigned char aBuffer[2])
{
	unsigned short val = aBuffer[0];
	val <<= 8;
	val |= aBuffer[1];

	return val;
}

void PackLong(unsigned long aVal, unsigned char aBuffer[4])
{
	unsigned long mask = 0xff000000;
	unsigned long byteVal = 0;

	for(int i=0; i<4; i++)
	{
		byteVal = aVal&mask;
		int shift = 8*(4-i-1);
		byteVal >>= shift;
		aBuffer[i] = byteVal;
		mask >>= 8;
	}
}

unsigned long UnpackLong(unsigned char buffer[4])
{
	unsigned long val = 0;
	for(int i=0; i<4; i++)
	{
		val <<= 8;
		val |= buffer[i];
	}
	return val;
}
