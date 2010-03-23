#pragma once
#include "Common\Utility\kpuVector.h"

class kpgLight
{
public:
	enum eLightType
	{
		eLT_Directional,
		eLT_Point
	};

	kpgLight(eLightType eType);
	~kpgLight(void);

	eLightType GetType() const							{ return m_eType; }
	const kpuVector& GetPositionDirection() const		{ return m_vPositionDireciton; }
	const kpuVector& GetColor() const					{ return m_vColor; }

	void SetDirection(const kpuVector& vDirection)		{ m_vPositionDireciton = vDirection; }
	void SetPosition(const kpuVector& vPosition)		{ m_vPositionDireciton = vPosition; }
	void SetColor(const kpuVector& vColor)				{ m_vColor = vColor; }

protected:
	eLightType		m_eType;
	kpuVector		m_vPositionDireciton;
	kpuVector		m_vColor;
};
