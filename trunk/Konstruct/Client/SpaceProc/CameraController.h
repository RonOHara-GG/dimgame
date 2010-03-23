#pragma once
#include "Common\Utility\kpuMatrix.h"

class CameraController
{
public:
	CameraController(void);
	virtual ~CameraController(void);

	virtual void Update() = 0;
	virtual const kpuMatrix& GetViewMatrix() = 0;

protected:
	kpuMatrix	m_mViewMatrix;
};
