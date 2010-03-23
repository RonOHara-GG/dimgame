#pragma once
#include "Common\Procedural\kppGeometry.h"

class kppBox :	public kppGeometry
{
public:
	kppBox(void);
	kppBox(float fWidth, float fHeight, float fDepth);
	virtual ~kppBox(void);

	void SetWidth(float fWidth)			{ m_fWidth = fWidth; }
	void SetHeight(float fHeight)		{ m_fHeight = fHeight; }
	void SetDepth(float fDepth)			{ m_fDepth = fDepth; }

	virtual void Build();

protected:
	float	m_fWidth;
	float	m_fHeight;
	float	m_fDepth;
};
