#include "StdAfx.h"
#include "SunAndMoon.h"

SunAndMoon::SunAndMoon(void)
{
	// Start at hour 0
	m_fTime = 0.0f;
}

SunAndMoon::~SunAndMoon(void)
{
}

void SunAndMoon::Update(float fDeltaSeconds)
{
	// Add the amount of time in hours to our time value
	m_fTime += (fDeltaSeconds / 60.0f) * (60.0f * 1.0f);
	if( m_fTime >= 24.0f )
		m_fTime -= 24.0f;

	// Set the sun direction and color
	if( m_fTime > 7.0f && m_fTime < 19.0f )
	{
		// Between 7am and 7pm the sunlight vector is valid
		float fPercentage = (m_fTime - 7.0f) / 12.0f;
		kpuVector vSunPoint = kpuv_OneX;
		vSunPoint.RotateZ(PI * fPercentage);
		m_vLightDirection = -vSunPoint;
		m_vLightColor.Set(1.0f, 1.0f, 0.5f, 1.0f);
	}
	else
	{
		// Between 7pm and 7am.  Set the light color to black
		// Direction wont matter since this light isnt contributing
		m_vLightColor = kpuv_Zero;
	}

	// Set the ambient color
	const kpuVector vNightColor(0.082f, 0.215f, 0.317f, 1.0f);
	const kpuVector vDayColor(0.284f, 0.374f, 0.441f, 1.0f);
	if( m_fTime > 5.5f && m_fTime < 8.5f )
	{
		// Dawn
		float fPercentage = (m_fTime - 5.5f) / 3.0f;
		m_vAmbientColor.Lerp(vNightColor, vDayColor, fPercentage); 
	}
	else if( m_fTime > 17.5f && m_fTime < 20.5f )
	{
		// Dusk
		float fPercentage = (m_fTime - 17.5f) / 3.0f;
		m_vAmbientColor.Lerp(vDayColor, vNightColor, fPercentage);
	}
	else if( m_fTime >= 8.5f &&m_fTime <= 17.5f )
	{
		// Day
		m_vAmbientColor = vDayColor;
	}
	else
	{
		// Night
		m_vAmbientColor = vNightColor; 
	}
}
