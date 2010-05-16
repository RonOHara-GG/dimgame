#pragma once
#include "Common\Utility\kpuVector.h"
#include "Common\Utility\kpuMatrix.h"
#include "kpuCollisionData.h"


class kpuBoundingVolume
{
public:

	enum eVolumeType
	{
		eVT_Sphere,
		eVT_Box,
		eVT_Capsule
	};

	kpuBoundingVolume(void);
	virtual ~kpuBoundingVolume(void);

	virtual void Intersects(kpuBoundingVolume* bOther, const kpuMatrix& matrix, kpuCollisionData& data) { }

	virtual void Transform(const kpuMatrix& matrix) { };

	

	eVolumeType GetType() { return m_eType; }

protected:

	eVolumeType m_eType;
};
