#pragma once

#include "Common/Utility/kpuVector.h"

class kpgModel;
class kpgRenderer;

class PlayerCharacter
{
public:
	PlayerCharacter(void);
	~PlayerCharacter(void);

	void Update();
	void Draw(kpgRenderer* pRenderer);

	void SetMoveTarget(const kpuVector&	vTarget)	{ m_vMoveTarget = vTarget; m_vMoveTarget.SetY(m_fHeightOffset); }

	kpuVector GetLocation();
	void SetLocation(const kpuVector& vNewLoc);

protected:
	kpgModel*		m_pModel;
	
	float			m_fHeightOffset;
	kpuVector		m_vMoveTarget;
};
