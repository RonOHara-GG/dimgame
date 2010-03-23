// LoginServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server/LoginServer/LoginServer.h"
#include "Common/Utility/kpuLinkedList.h"
#include "Common/Network/kpnListenSocket.h"
#include "Common/Network/kpnServer.h"
#include "Common/Network/kpnBaseSocket.h"
#include "Common/Network/kpnClient.h"

extern DispatchFunction g_pClientHandlers[];
extern DispatchFunction g_pWorldHandlers[];
extern DispatchFunction g_pDBHandlers[];

void UpdateClients(kpuLinkedList& lClientList, kpnServer* pWorldServer, kpnServer* pDatabaseServer)
{
	// Handle all client connections
	kpuLinkedList* pIter = lClientList.Next();
	while( pIter )
	{
		kpnClient* pClient = (kpnClient*)pIter->GetPointer();
//		if( !pClient->Update() )
		{
			// Connection is closed, remove it from the list
			kpuLinkedList* pKill = pIter;
			pIter = pIter->Prev();
			delete pKill;
		}

		pIter = pIter->Next();
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	kpnBaseSocket::Initialize();

	kpuLinkedList lClientList;

	// Find the world server
	kpnServer* pWorldServer = kpnServer::FindWorldServer();
	if( !pWorldServer )
	{
		printf("ERROR: Unable to find world server.  Cannot continue\n");
		return -1;
	}

	// Find the database server
	kpnServer* pDatabaseServer = kpnServer::FindDatabaseServer();
	if( !pDatabaseServer )
	{
		printf("ERROR: Unable to find database server.  Cannot continue\n");
		delete pWorldServer;
		return -2;
	}

	// Create our listen socket
	kpnBaseSocket* pListenSocket = new kpnBaseSocket(LOGIN_SERVER_PORT);

	// Loop forever
	while( 1 )
	{
		// Update listen socket
		pListenSocket->Update(g_pClientHandlers);

		// Update World server connection
		pWorldServer->Update(g_pWorldHandlers);

		// Update database server connection
		pDatabaseServer->Update(g_pDBHandlers);

		// Update client connections
		UpdateClients(lClientList, pWorldServer, pDatabaseServer);

		// Play nice with others
		Sleep(1);
	}

	while( lClientList.Next() )
		delete lClientList.Next();

	delete pDatabaseServer;
	delete pWorldServer;
	delete pListenSocket;

	kpnBaseSocket::Shutdown();
	return 0;
}

