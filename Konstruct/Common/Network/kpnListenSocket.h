#pragma once
#include "kpnbasesocket.h"
#include "Common/Utility/kpuLinkedList.h"

class kpnListenSocket : public kpnBaseSocket
{
public:
	kpnListenSocket(int nPort);
	virtual ~kpnListenSocket(void);

	void Update(kpuLinkedList& lNewConnections, DispatchFunction* pDispatchFunctions);

protected:
};
