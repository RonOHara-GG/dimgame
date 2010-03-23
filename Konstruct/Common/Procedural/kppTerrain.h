#pragma once
#include "Common\Procedural\kppGeometry.h"

class kppTerrain : public kppGeometry
{
public:
	kppTerrain(void);
	kppTerrain(int iPolesWide, float fWidthBetweenPoles, int iPolesHigh, float fHeightBetweenPoles);
	virtual ~kppTerrain(void);

	void SetPoles(int iPolesWide, float fWidthBetweenPoles, int iPolesHigh, float fHeightBetweenPoles)	{ m_iPolesWide = iPolesWide; m_iPolesHigh = iPolesHigh; m_fPoleWidth = fWidthBetweenPoles; m_fPoleHeight = fHeightBetweenPoles; }

	virtual void Build();

protected:
	int		m_iPolesWide;
	int		m_iPolesHigh;
	float	m_fPoleWidth;
	float	m_fPoleHeight;
};
