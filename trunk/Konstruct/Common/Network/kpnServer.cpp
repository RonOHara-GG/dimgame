#include "stdafx.h"
#include "kpnServer.h"
#include "kpnBroadcastSocket.h"
#include "kpnGPackets.h"
#include "kpnWSPackets.h"
#include "kpnDBPackets.h"
#include "kpnWorldServer.h"
#include "kpnDatabaseServer.h"
#include "kpnBaseSocket.h"
#include "Common\Utility\kpuStopwatch.h"

static u32 s_unServerAddress = 0xFFFFFFFF;
static int s_nServerPort = -1;
void IdentityHandler(kpnBaseSocket* pServer, kpnPacketInfo* pPacket)
{
	kpnGPIdentity* pIdentity = (kpnGPIdentity*)pPacket->m_pPacket;
	s_unServerAddress = pIdentity->Address();
	s_nServerPort = pIdentity->Port();
}

void ConnectedHandler(kpnBaseSocket* pSvr, kpnPacketInfo* pPacket)
{
	kpnServer* pServer = (kpnServer*)pSvr;
	pServer->SetState(kpnServer::eSS_Connected);
}

DispatchFunction s_identityHandler[] = 
{
	/* GPID_IDENTITY */		IdentityHandler,
	/* GPID_IDENTIFY */		0,
	/* GPID_CONNECT */		0,
	/* GPID_CONNECTED */	ConnectedHandler,
};

kpnServer::kpnServer() : kpnBaseSocket(0)
{
	m_eState = eSS_None;
}

kpnServer::~kpnServer()
{
}

kpnServer* kpnServer::FindWorldServer()
{
	kpnGPIdentitfy pktId;

	kpnBroadcastSocket bcast;
	bcast.Send(&pktId, WORLD_SERVER_PORT);

	s_unServerAddress = 0xFFFFFFFF;
	s_nServerPort = -1;

	kpuStopwatch sw;
	sw.SetSeconds(30);
	while( !sw.Expired() )
	{
		bcast.Update(s_identityHandler);

		if( s_unServerAddress != 0xFFFFFFFF )
		{
			kpnWorldServer* pWorldServer = new kpnWorldServer();
			pWorldServer->Connect(s_unServerAddress, s_nServerPort);
			sw.SetSeconds(30);
			while( !sw.Expired() )
			{
				pWorldServer->Update(s_identityHandler);

				if( pWorldServer->GetState() == kpnServer::eSS_Connected )
				{
					pWorldServer->SetState(kpnServer::eSS_Ready);
					return pWorldServer;
				}
			}
			sw.SetSeconds(2);
		}
	}

	// no one responded that we could connect to :(
	return 0;
}

kpnServer* kpnServer::FindDatabaseServer()
{
	kpnGPIdentitfy pktId;

	kpnBroadcastSocket bcast;
	bcast.Send(&pktId, DATABASE_SERVER_PORT);

	s_unServerAddress = 0xFFFFFFFF;
	s_nServerPort = -1;

	kpuStopwatch sw;
	sw.SetSeconds(30);
	while( !sw.Expired() )
	{
		bcast.Update(s_identityHandler);

		if( s_unServerAddress != 0xFFFFFFFF )
		{
			kpnDatabaseServer* pDatabaseServer = new kpnDatabaseServer();
			pDatabaseServer->Connect(s_unServerAddress, s_nServerPort);
			sw.SetSeconds(30);
			while( !sw.Expired() )
			{
				pDatabaseServer->Update(s_identityHandler);

				if( pDatabaseServer->GetState() == kpnServer::eSS_Connected )
				{
					pDatabaseServer->SetState(kpnServer::eSS_Ready);
					return pDatabaseServer;
				}
			}
			sw.SetSeconds(2);
		}
	}

	// no one responded that we could connect to :(
	return 0;
}

void kpnServer::CreateConnectionFromAddress(u32 unAddress, int nPort)
{
	SetAddress(unAddress);
	SetPort(nPort);
}

bool kpnServer::Connect(u32 unServerAddress, int nPort)
{
	// Send a connection request to the server
	kpnGPConnect conn;
	kpnBaseSocket::Send(&conn, unServerAddress, nPort, false);

	// Mark our state and leave
	m_eState = eSS_Connecting;
	return true;
}

void kpnServer::Send(kpnPacket* pPacket)
{
	kpnBaseSocket::Send(pPacket, Address(), Port());
}