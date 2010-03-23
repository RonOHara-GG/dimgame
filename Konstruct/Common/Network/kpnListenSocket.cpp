#include "StdAfx.h"
#include "kpnListenSocket.h"
#include "kpnClient.h"

kpnListenSocket::kpnListenSocket(int nPort) : kpnBaseSocket(nPort)
{
	
}

kpnListenSocket::~kpnListenSocket(void)
{
}

void kpnListenSocket::Update(kpuLinkedList& lNewConnections, DispatchFunction* pDispatchFunctions)
{
	// Check for new connections
	sockaddr_in newAddr;
	int newAddrLen = sizeof(sockaddr_in);
	SOCKET sNewSocket = accept(m_sSocket, (sockaddr*)&newAddr, &newAddrLen);
	if( sNewSocket != INVALID_SOCKET )
	{
		kpnBaseSocket clientSocket(sNewSocket, newAddr.sin_addr.S_un.S_addr, newAddr.sin_port);
		kpnClient* pClient = new kpnClient(clientSocket);
		lNewConnections.AddTail(pClient);
	}

	// Call the baseclass update
	kpnBaseSocket::Update(pDispatchFunctions);
}