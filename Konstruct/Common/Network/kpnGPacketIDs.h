#pragma once

#define GPID(x)				(x)
#define GPID_IDENTITY		GPID(0)
#define GPID_IDENTIFY		GPID(1 + GPID_IDENTITY)
#define GPID_CONNECT		GPID(1 + GPID_IDENTIFY)
#define GPID_CONNECTED		GPID(1 + GPID_CONNECT)