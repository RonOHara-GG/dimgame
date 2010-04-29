#include "StdAfx.h"
#include "kpuBoundingBox.h"

kpuBoundingBox::kpuBoundingBox(kpuVector vMin ,kpuVector vMax)
{
	m_vMax = vMax;
	m_vMin = vMin;
}

kpuBoundingBox::~kpuBoundingBox(void)
{
}

bool kpuBoundingBox::Intersects(kpuBoundingBox &bBox)
{
	if(m_vMin.GetX() > bBox.m_vMax.GetX() )
		return false;
	if(m_vMax.GetX() < bBox.m_vMax.GetX() )
		return false;

	if(m_vMin.GetY() > bBox.m_vMax.GetY() )
		return false;
	if(m_vMax.GetY() < bBox.m_vMax.GetY() )
		return false;

	if(m_vMin.GetZ() > bBox.m_vMax.GetZ() )
		return false;
	if(m_vMax.GetZ() < bBox.m_vMax.GetZ() )
		return false;
	
			
	return true;
}

bool kpuBoundingBox::Contains(kpuBoundingBox& bBox)
{
	//if the this min is less than bbox min and bbox min is less than max and max is greater than bbox max and bbox max is greater than this min then
	//bbox is inside of this box
	if ( m_vMin.GetX() <= bBox.GetMin().GetX() && m_vMin.GetY() <= bBox.GetMin().GetY() && m_vMin.GetZ() <= bBox.GetMin().GetZ() &&
		m_vMax.GetX() >= bBox.GetMin().GetX() && m_vMax.GetY() >= bBox.GetMin().GetY() && m_vMax.GetZ() >= bBox.GetMin().GetZ() )
	{
		if ( m_vMin.GetX() <= bBox.GetMax().GetX() && m_vMin.GetY() <= bBox.GetMax().GetY() && m_vMin.GetZ() <= bBox.GetMax().GetZ() &&
		m_vMax.GetX() >= bBox.GetMax().GetX() && m_vMax.GetY() >= bBox.GetMax().GetY() && m_vMax.GetZ() >= bBox.GetMax().GetZ() )
		{
			return true;
		}
	}


	return false;
}

bool kpuBoundingBox::Intersects(kpuBoundingSphere &bSphere)
{

	return false;
}