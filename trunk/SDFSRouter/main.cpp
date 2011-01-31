/*
 * main.cpp
 *
 *  Created on: Jan 22, 2011
 *      Author: shashank
 */

#include "Router.h"
#include "DNS.h"
#include "DHCP.h"
#include "SDFSDebug.h"

#include <cstdlib>
#include <iostream>
using namespace std;

unsigned short CDebug::iClientId;

int main()
{

	int routerPort = 8000;
	int dnsPort = 10002;
	int dhcpPort = 10001;

	CDNS dns(dnsPort);
	CDHCP dhcp(dhcpPort,&dns);
	CRouter router(routerPort,&dns,&dhcp);

	dns.StartDNSServer();
	dhcp.StartDHCPServer();
	router.RunRouter();

	return 0;
}
