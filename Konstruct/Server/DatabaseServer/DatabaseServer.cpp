// DatabaseServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Common/Network/kpnServer.h"
#include "Common/Network/kpnListenSocket.h"
#include "Common/Utility/kpuLinkedList.h"

extern DispatchFunction s_pPacketHandlers[];
kpuLinkedList g_lServerList;

int _tmain(int argc, _TCHAR* argv[])
{
	kpnBaseSocket::Initialize();

	kpnBaseSocket* pSocket = new kpnBaseSocket(DATABASE_SERVER_PORT);

	printf("Database Server running on port: %d\n", DATABASE_SERVER_PORT);

	while( 1 )
	{
		// Update the bound socket
		pSocket->Update(s_pPacketHandlers);

		// Update the server lists
		kpuLinkedList* pIter = g_lServerList.Next();
		while( pIter )
		{
			kpnServer* pServer = (kpnServer*)pIter->GetPointer();
			pServer->Update(s_pPacketHandlers);
			if( pServer->GetState() == kpnServer::eSS_Closed )
			{
				// Connection closed, remove it from the list
				kpuLinkedList* pKill = pIter;
				pIter = pIter->Prev();
				delete pServer;
				delete pKill;
			}
			
			pIter = pIter->Next();
		}

		// Play nice with others
		Sleep(1);
	}

	kpnBaseSocket::Shutdown();
	return 0;
}

