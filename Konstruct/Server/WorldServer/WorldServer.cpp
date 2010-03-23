// WorldServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Common/Network/kpnListenSocket.h"
#include "Common/Network/kpnServer.h"
#include "Common/Utility/kpuLinkedList.h"
#include "Common/Network/kpnDatabaseServer.h"
#include "Common/Network/kpnBroadcastSocket.h"
#include "Common/Network/kpnDBPackets.h"

kpuLinkedList g_lServerList;
extern DispatchFunction s_pPacketHandlers[];

int _tmain(int argc, _TCHAR* argv[])
{
	kpnBaseSocket::Initialize();

	// Find the database server
	kpnServer* pDatabaseServer = kpnServer::FindDatabaseServer();
	if( !pDatabaseServer )
	{
		printf("Failed to connect to the database server\n");
		return 0;
	}

	// Start Listening for other server connections
	kpnBaseSocket* pListen = new kpnBaseSocket(WORLD_SERVER_PORT);
	printf("World Server running on port: %d\n", WORLD_SERVER_PORT);

	// Loop forever
	while( 1 )
	{
		// Process incoming connections
		pListen->Update(s_pPacketHandlers);

		// Process all the servers
		kpuLinkedList* pServerNode = g_lServerList.Next();
		while( pServerNode )
		{
			kpnServer* pServer = (kpnServer*)pServerNode->GetPointer();
//			pServer->Update();
			pServerNode = pServerNode->Next();
		}

		// Be nice to other processes
		Sleep(1);
	}

	delete pListen;
	delete pDatabaseServer;

	kpnBaseSocket::Shutdown();
	return 0;
}

