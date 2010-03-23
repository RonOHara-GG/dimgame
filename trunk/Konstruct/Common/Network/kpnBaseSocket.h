#pragma once

#include <winsock.h>
#include "kpnPacket.h"
#include "Common/Utility/kpuLinkedList.h"

#define MAX_PACKET_SIZE			512
#define PACKET_RESEND_SECONDS	10


class kpnBaseSocket;

typedef void (*DispatchFunction)(kpnBaseSocket* pSocket, kpnPacketInfo* pPacket);

class kpnBaseSocket
{
public:
	kpnBaseSocket(int nPort);
	kpnBaseSocket(SOCKET sSocket, u32 unAddress, int nPort);
	virtual ~kpnBaseSocket(void);

	static void Initialize();
	static void Shutdown();

	void SetAddress(const char* szAddress);
	void SetAddress(u32 unAddress);
	void SetPort(int nPort);

	u32		Address() const				{ return m_unAddress; }
	int		Port() const				{ return m_nPort; }

	u32		GetLocalIP() const;

	virtual void Send(kpnPacket* pPacket, u32 unAddress, int nPort, bool bStream = true);
	virtual bool ReadPacket(char* pOutBuffer, int& nOutBufferSize, sockaddr_in* pFrom = 0);

	virtual void Update(DispatchFunction* pDispatchFunctions);

	void PrintError() const;

private:
	void SendInternal(kpnPacket* pPacket, u32 unAddress, int nPort);
	void SendLowLevel(void* pData, int nSize, u32 unAddress, int nPort);
	void Acknowledge(kpnPacket* pPacket);
	void SendAcknowledgement(kpnPacketInfo* pPacket);

protected:
	u32				m_unAddress;
	int				m_nPort;
	SOCKET			m_sSocket;
	bool			m_bOwnsSocketHandle;

	kpuLinkedList	m_lPackets;
	kpuLinkedList	m_lPacketSendList;
	u32				m_unCurrentReadPacket;
	u32				m_unCurrentSendPacket;
};
