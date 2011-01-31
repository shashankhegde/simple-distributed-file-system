/*
 * main.cpp
 *
 *  Created on: Jan 13, 2011
 *      Author: shashank
 */

#include "SDFSConnector.h"
#include "SDFSClient.h"

#include <cstdlib>
#include <iostream>
using namespace std;

/*

Connectors and the Client communicate via a message queue.

*/

#include "SDFSDebug.h"
unsigned short CDebug::iClientId = -1;

int main(int argc, char* argv[])
{

	if(argc != 3)
	{
		cout << "Usage : " << argv[0] << " <clientport> <router ip>\n";
		exit(-1);
	}



	unsigned short clientPort = atoi(argv[1]);

	SDFSClient client(0,clientPort,argv[2]);
	client.RunClient();

	return 0;
}

