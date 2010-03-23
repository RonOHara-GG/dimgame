#pragma once

#include "kpnBaseSocket.h"

class kpnBaseComm
{
public:
	kpnBaseComm()		{}
	~kpnBaseComm()		{}

protected:
	kpnBaseSocket		m_sSocket;
};