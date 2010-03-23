#pragma once

#include "kpnBaseSocket.h"
class kpnPacket;


#define LOGIN_SERVER_PORT		8873
#define WORLD_SERVER_PORT		8874
#define DATABASE_SERVER_PORT	8875

// The kpnServer class is the base communication class for communicating with other servers in the cluster.
class kpnServer : public kpnBaseSocket
{
public:
	enum eServerState
	{
		eSS_None = 0,
		eSS_Connecting,
		eSS_Connected,
		eSS_Ready,
		eSS_Closed,

		eSS_Last
	};
	kpnServer();
	virtual ~kpnServer();

	void CreateConnectionFromAddress(u32 unAddress, int nPort);
	bool Connect(u32 unServerAddress, int nPort);

	void Send(kpnPacket* pPacket);

	eServerState	GetState() const			{ return m_eState; }
	void			SetState(eServerState s)	{ m_eState = s; }

	static kpnServer*	FindWorldServer();
	static kpnServer*	FindDatabaseServer();

protected:
	eServerState			m_eState;
};