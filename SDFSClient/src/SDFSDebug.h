/*
 * SDFSDebug.h
 *
 *  Created on: Jan 13, 2011
 *      Author: shashank
 */

#ifndef SDFSDEBUG_H_
#define SDFSDEBUG_H_

#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

class CDebug
{
public:
	static void SetClientId(unsigned short aClientId)
	{
		iClientId = aClientId;
	}

	static void Log(const char* aLog)
	{
		std::cout << "[" << iClientId << "] " << "LOG: " << aLog << std::endl;
	}

	static void Error(const char* aError)
	{
		std::cout << "[" << iClientId << "] " << "ERROR: " << aError << std::endl;
		perror("");
	}

private:
	static unsigned short iClientId;
};


#endif /* SDFSDEBUG_H_ */
