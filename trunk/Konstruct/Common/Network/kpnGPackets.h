#pragma once

#include "kpnGPacketIDs.h"

class kpnGPIdentitfy : public kpnPacket
{
public:
	kpnGPIdentitfy() : kpnPacket(GPID_IDENTIFY, sizeof(kpnGPIdentitfy), 0)	{}

protected:

};

class kpnGPIdentity : public kpnPacket
{
public:
	kpnGPIdentity() : kpnPacket(GPID_IDENTITY, sizeof(kpnGPIdentity), sizeof(kpnGPIdentity) - sizeof(kpnPacket))		{}

	u32				Address() const						{ return m_unAddress; }
	int				Port() const						{ return m_nPort; }

	void			SetAddress(u32 unAddress)			{ m_unAddress = unAddress; }
	void			SetPort(int nPort)					{ m_nPort = nPort; }

protected:
	u32		m_unAddress;
	int		m_nPort;
};

class kpnGPConnect : public kpnPacket
{
public:
	kpnGPConnect() : kpnPacket(GPID_CONNECT, sizeof(kpnGPConnect), 0)		{}
};

class kpnGPConnected : public kpnPacket
{
public:
	kpnGPConnected() : kpnPacket(GPID_CONNECTED, sizeof(kpnGPConnected), 0)	{}
};