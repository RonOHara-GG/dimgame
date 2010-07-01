#include "StdAfx.h"
#include "Common/Utility/kpuCameraController.h"

kpuCameraController::kpuCameraController(void)
{
	m_vLocation = kpuv_Zero;
	m_vLookAt = kpuv_Zero;
	m_vUp = kpuv_OneY;
	SetView(m_vLocation, m_vLookAt, m_vUp);
}

kpuCameraController::~kpuCameraController(void)
{
}

void kpuCameraController::SetView(const kpuVector& vLocation, const kpuVector& vLookAt, const kpuVector& vUp)
{
	m_mViewMatrix.LookAt(vLocation, vLookAt, vUp);
}

void kpuCameraController::Update()
{
	m_mViewMatrix.LookAt(m_vLocation, m_vLookAt, m_vUp);
}