#pragma once
#include "Common/Utility/kpuCameraController.h"

class kpuThreeQuarterTopDownCamera : public kpuCameraController
{
public:
	kpuThreeQuarterTopDownCamera(void);
	kpuThreeQuarterTopDownCamera(const kpuVector& vLocation, const kpuVector& vLookAt, const kpuVector& vUp);
	virtual ~kpuThreeQuarterTopDownCamera(void);

	virtual void Update();
	virtual const kpuMatrix& GetViewMatrix()	{ return m_mViewMatrix; }

protected:
	kpuVector	m_vViewDir;
};
