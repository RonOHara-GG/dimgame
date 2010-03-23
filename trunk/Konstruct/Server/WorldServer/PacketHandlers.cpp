#include "Stdafx.h"
#include "Common/Network/kpnServer.h"
#include "Common/Network/kpnBaseSocket.h"
#include "Common/Network/kpnGPackets.h"

extern kpuLinkedList g_lServerList;

void WSIdentifyHandler(kpnBaseSocket* pSocket, kpnPacketInfo* pPacket)
{
	kpnGPIdentity identity;
	identity.SetAddress(pSocket->GetLocalIP());
	identity.SetPort(WORLD_SERVER_PORT);

	union addr
	{
		u8	baddr[4];
		u32	unaddr;
	};
	addr a;
	a.unaddr = pPacket->m_unAddress;
	printf("Sending Identity to %d.%d.%d.%d\n", a.baddr[0], a.baddr[1], a.baddr[2], a.baddr[3]);
	pSocket->Send(&identity, pPacket->m_unAddress, pPacket->m_nPort);
}

void WSConnectHandler(kpnBaseSocket* pSocket, kpnPacketInfo* pPacket)
{
	// Someone wants to connect to us, setup a new socket for them
	kpnServer* pNewServer = new kpnServer();
	pNewServer->CreateConnectionFromAddress(pPacket->m_unAddress, pPacket->m_nPort);
	pNewServer->SetState(kpnServer::eSS_Ready);

	// Add the socket to our list
	g_lServerList.AddTail(pNewServer);

	// Send a connected packet back to the new server
	kpnGPConnected connected;
	pNewServer->Send(&connected);

	union addr
	{
		u8	baddr[4];
		u32	unaddr;
	};
	addr a;
	a.unaddr = pPacket->m_unAddress;
	printf("Accepted new connection from %d.%d.%d.%d:%d\n", a.baddr[0], a.baddr[1], a.baddr[2], a.baddr[3], pPacket->m_nPort);
}

DispatchFunction s_pPacketHandlers[] = {
	/* GPID_IDENTITY */		0,
	/* GPID_IDENTIFY */		WSIdentifyHandler,
	/* GPID_CONNECT */		WSConnectHandler,
	/* GPID_CONNECTED */	0,
};