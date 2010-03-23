#pragma once
#include "Common\Procedural\kppGeometry.h"
#include "Common\Utility\kpuVector.h"

class kppPlane : public kppGeometry
{
public:
	kppPlane(void);
	kppPlane(float fWidth, float fHeight, const kpuVector& vNormal);
	virtual ~kppPlane(void);

	void SetNormal(const kpuVector& vNormal)		{ m_vNormal = vNormal; m_vNormal.Normalize(); }
	void SetWidth(float fWidth)						{ m_fWidth = fWidth; }
	void SetHeight(float fHeight)					{ m_fHeight = fHeight; }

	virtual void Build();

protected:
	kpuVector	m_vNormal;
	float		m_fWidth;
	float		m_fHeight;
};
