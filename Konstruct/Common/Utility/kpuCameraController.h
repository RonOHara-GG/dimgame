#pragma once
#include "Common\Utility\kpuMatrix.h"

class kpuCameraController
{
public:
	kpuCameraController(void);
	virtual ~kpuCameraController(void);

	virtual void Update() = 0;
	virtual const kpuMatrix& GetViewMatrix() = 0;
	virtual void SetView(const kpuVector& vLocation, const kpuVector& vLookAt, const kpuVector& vUp);
	virtual void SetLocation(const kpuVector& vLocation)		{ m_vLocation = vLocation; SetView(m_vLocation, m_vLookAt, m_vUp); }
	virtual void SetLookAt(const kpuVector& vLookAt)			{ m_vLookAt = vLookAt; SetView(m_vLocation, m_vLookAt, m_vUp); }
	virtual void SetUp(const kpuVector& vUp)					{ m_vUp = vUp; SetView(m_vLocation, m_vLookAt, m_vUp); }

protected:
	kpuMatrix	m_mViewMatrix;
	kpuVector	m_vLocation;
	kpuVector	m_vLookAt;
	kpuVector	m_vUp;
};
