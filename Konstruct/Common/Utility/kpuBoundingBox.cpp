#include "StdAfx.h"
#include "kpuBoundingBox.h"

kpuBoundingBox::kpuBoundingBox(float fX, float fZ, float fWidth, float fHeight)
{
	m_vMax = kpuVector(fX + fWidth, 0.0f, fZ + fHeight, 0.0f);
	m_vMin = kpuVector(fX, 0.0f, fZ, 0.0f);
}

kpuBoundingBox::~kpuBoundingBox(void)
{
}

bool kpuBoundingBox::Intersects(kpuBoundingBox &bBox)
{
	//Check top of this against bottom of bBox
	if( m_vMin.GetZ() < bBox.m_vMax.GetZ() && m_vMin.GetX() < bBox.m_vMax.GetX() && m_vMax.GetX() > bBox.m_vMin.GetX() && m_vMax.GetZ() > bBox.m_vMin.GetZ() )
		return true;

	//Check bottom of this against top of bBox
	if( m_vMax.GetZ() > bBox.m_vMin.GetZ() && m_vMin.GetX() < bBox.m_vMax.GetX() && m_vMax.GetX() > bBox.m_vMin.GetX() && m_vMin.GetZ() < bBox.m_vMax.GetZ() )
		return true;

	//Check left of this against right of bBox
	if( m_vMin.GetX() < bBox.m_vMax.GetX() && m_vMin.GetZ() < bBox.m_vMax.GetZ() && m_vMax.GetZ() > bBox.m_vMin.GetZ() && m_vMax.GetX() > bBox.m_vMin.GetX() )
		return true;

	//Check right of this against left of bBox
	if( m_vMax.GetX() > bBox.m_vMin.GetX() && m_vMin.GetZ() < bBox.m_vMax.GetZ() && m_vMax.GetZ() > bBox.m_vMin.GetZ() && m_vMin.GetX() < bBox.m_vMax.GetX() )
		return true;
			
	return false;
}

bool kpuBoundingBox::Intersects(kpuBoundingSphere &bSphere)
{

	return false;
}
