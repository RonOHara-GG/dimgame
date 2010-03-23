#include "StdAfx.h"
#include "Common/Network/kpnBaseSocket.h"



DispatchFunction g_pClientHandlers[] = 
{
	/* GPID_IDENTITY */		0,
	/* GPID_IDENTIFY */		0,
	/* GPID_CONNECT */		0,
	/* GPID_CONNECTED */	0,
};

DispatchFunction g_pWorldHandlers[] = 
{
	/* GPID_IDENTITY */		0,
	/* GPID_IDENTIFY */		0,
	/* GPID_CONNECT */		0,
	/* GPID_CONNECTED */	0,
};

DispatchFunction g_pDBHandlers[] = 
{
	/* GPID_IDENTITY */		0,
	/* GPID_IDENTIFY */		0,
	/* GPID_CONNECT */		0,
	/* GPID_CONNECTED */	0,
};