#include "StdAfx.h"
#include "Common\Graphics\kpgGeometryInstance.h"

kpgGeometryInstance::kpgGeometryInstance(kpgGeometry* pGeometry)
{
	m_pGeometry = pGeometry;
	m_mLocalToWorld.Identity();
}

kpgGeometryInstance::kpgGeometryInstance(kpgGeometry* pGeometry, const kpuMatrix& mLocalToWorld)
{
	m_pGeometry = pGeometry;
	m_mLocalToWorld = mLocalToWorld;
}

kpgGeometryInstance::~kpgGeometryInstance(void)
{
}
