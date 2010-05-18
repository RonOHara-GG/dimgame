#pragma once

class NPC;

class AIControl
{
public:
	AIControl(void);
	virtual ~AIControl(void);
	virtual void Update(NPC* pNpc) {}

};
