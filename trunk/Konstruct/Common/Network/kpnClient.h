#pragma once

#include "kpnBaseSocket.h"

class kpnClient : public kpnBaseSocket
{
public:
	kpnClient(kpnBaseSocket& socket);
	~kpnClient(void);

	bool Update();

protected:
};
