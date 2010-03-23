// TestClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Common/Network/kpnBaseSocket.h"
#include "Common/Network/kpnServer.h"

#define LOGIN_SERVER_ADDRESS		"127.0.0.1"

int _tmain(int argc, _TCHAR* argv[])
{
	kpnBaseSocket::Initialize();

	// Connect to login server
	kpnServer* pServer = new kpnServer();
	if( !pServer->Connect(inet_addr(LOGIN_SERVER_ADDRESS), LOGIN_SERVER_PORT) )
	{
		printf("Failed to connect to login server\n");
		kpnBaseSocket::Shutdown();
		return -1;
	}

	// Authenticate

	// Close connection

	kpnBaseSocket::Shutdown();
	return 0;
}

