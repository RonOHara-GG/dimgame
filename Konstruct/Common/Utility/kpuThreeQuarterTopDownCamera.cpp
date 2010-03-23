#include "StdAfx.h"
#include "Common/Utility/kpuThreeQuarterTopDownCamera.h"

kpuThreeQuarterTopDownCamera::kpuThreeQuarterTopDownCamera(void)
{
	SetLocation(kpuVector(10.0f, 10.0f, 0.0f, 0.0f));
	m_vViewDir = m_vLookAt - m_vLocation;
}

kpuThreeQuarterTopDownCamera::kpuThreeQuarterTopDownCamera(const kpuVector& vLocation, const kpuVector& vLookAt, const kpuVector& vUp)
{
	m_vLocation = vLocation;
	m_vLookAt = vLookAt;
	m_vUp = vUp;
	m_vViewDir = m_vLookAt - m_vLocation;
	SetView(vLocation, vLookAt, vUp);
}

kpuThreeQuarterTopDownCamera::~kpuThreeQuarterTopDownCamera(void)
{
}

void kpuThreeQuarterTopDownCamera::Update()
{
	SetLocation(m_vLookAt - m_vViewDir);
}