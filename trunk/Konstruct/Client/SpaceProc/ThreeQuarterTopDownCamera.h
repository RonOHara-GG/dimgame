#pragma once
#include "CameraController.h"

class ThreeQuarterTopDownCamera : public CameraController
{
public:
	ThreeQuarterTopDownCamera(void);
	virtual ~ThreeQuarterTopDownCamera(void);

	virtual void Update();
	virtual const kpuMatrix& GetViewMatrix()	{ return m_mViewMatrix; }

protected:
};
