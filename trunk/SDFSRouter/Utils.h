/*
 * Utils.h
 *
 *  Created on: Jan 25, 2011
 *      Author: shashank
 */

#ifndef UTILS_H_
#define UTILS_H_

void PackShort(unsigned short aVal, unsigned char aBuffer[2]);
unsigned short UnpackShort(unsigned char aBuffer[2]);

void PackLong(unsigned long aVal, unsigned char aBuffer[4]);
unsigned long UnpackLong(unsigned char buffer[4]);

#endif /* UTILS_H_ */
