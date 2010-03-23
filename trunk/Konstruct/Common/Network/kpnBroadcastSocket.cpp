#include "StdAfx.h"
#include "kpnBroadcastSocket.h"

kpnBroadcastSocket::kpnBroadcastSocket(void) : kpnBaseSocket(0)
{
	BOOL val = TRUE;
	setsockopt(m_sSocket, SOL_SOCKET, SO_BROADCAST, (const char*)&val, sizeof(val));
}

kpnBroadcastSocket::~kpnBroadcastSocket(void)
{
}

void kpnBroadcastSocket::Send(kpnPacket* pPacket, int nPort)
{
	sockaddr_in to;
	ZeroMemory(&to, sizeof(sockaddr_in));
	to.sin_family = AF_INET;
#ifdef STATIC_SERVER_ADDRESS
	to.sin_addr.S_un.S_addr = inet_addr(STATIC_SERVER_ADDRESS);
#else
	to.sin_addr.S_un.S_addr = INADDR_BROADCAST;
#endif
	to.sin_port = nPort;
	pPacket->SetSequenceNumber(NON_STREAM_SEQUENCE);
	sendto(m_sSocket, (const char*)pPacket, pPacket->Size(), 0, (sockaddr*)&to, sizeof(sockaddr_in));
}