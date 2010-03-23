#pragma once
#include "kpnBaseSocket.h"
#include "kpnPacket.h"

#define STATIC_SERVER_ADDRESS		"127.0.0.1"

class kpnBroadcastSocket : public kpnBaseSocket
{
public:
	kpnBroadcastSocket(void);
	virtual ~kpnBroadcastSocket(void);

	virtual void Send(kpnPacket* pPacket, int nPort);
};
