#pragma once
#include "Common/Utility/kpuBoundingVolume.h"
#include "kpuBoundingCapsule.h"

class kpuBoundingBox: public kpuBoundingVolume
{
public:
	kpuBoundingBox(kpuVector vMin = kpuv_OneW ,kpuVector = kpuv_OneW );
	kpuBoundingBox(const kpuBoundingBox& bBox);
	~kpuBoundingBox(void);

	void operator=(const kpuBoundingBox& bBox);

	void Intersects(kpuBoundingVolume* bOther, const kpuMatrix& matrix, kpuCollisionData& data);
	void Transform(const kpuMatrix& matrix);
	

	bool Contains2D(kpuBoundingBox& bBox);
	bool Contains2D(kpuBoundingCapsule& bCapsule);

	kpuVector GetMax()	{ return m_vMax; }
	kpuVector GetMin()	{ return m_vMin; }
	void	  SetMin(kpuVector vMin)  { m_vMin = vMin; }
	void	  SetMax(kpuVector vMax) { m_vMax = vMax; }

	void Move(kpuVector vVel)	{ m_vMin +=  vVel; m_vMax += vVel; }

private:
	kpuVector m_vMax;
	kpuVector m_vMin;

};
