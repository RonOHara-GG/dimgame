#pragma once

#include "Common/Utility/kpuVector.h"
#include "Common/Utility/kpuPhysicalObject.h"

class kpgRenderer;

class GameObject:public kpuPhysicalObject
{
public:
	GameObject(void);
	virtual ~GameObject(void);

	

};

#define WALL		( 1 << 0 )
#define ATTACKABLE	( 1 << 1 )
#define PLAYER		( 1 << 2 )
#define ENEMY		( 1 << 3 )
#define NPC			( 1 << 4 )