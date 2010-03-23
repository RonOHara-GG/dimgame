#pragma once
#include "Common\Utility\kpuVector.h"

class SunAndMoon
{
public:
	SunAndMoon(void);
	~SunAndMoon(void);

	void Update(float fDeltaSeconds = 0.01666667f);

	void SetTime(float fTimeInHours)				{ m_fTime = fTimeInHours; }

	const kpuVector& GetLightDirection() const		{ return m_vLightDirection; }
	const kpuVector& GetLightColor() const			{ return m_vLightColor; }
	const kpuVector& GetAmbientColor() const		{ return m_vAmbientColor; }	

protected:
	float		m_fTime;
	kpuVector	m_vLightDirection;
	kpuVector	m_vLightColor;
	kpuVector	m_vAmbientColor;
};
