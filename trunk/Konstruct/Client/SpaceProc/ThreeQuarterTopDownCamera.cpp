#include "StdAfx.h"
#include "ThreeQuarterTopDownCamera.h"

ThreeQuarterTopDownCamera::ThreeQuarterTopDownCamera(void)
{
	m_mViewMatrix.LookAt(kpuVector(5.0f, 0.0f, 0.0f, 0.0f), kpuv_Zero, kpuv_OneY);
}

ThreeQuarterTopDownCamera::~ThreeQuarterTopDownCamera(void)
{
}

void ThreeQuarterTopDownCamera::Update()
{
}
