#pragma once
#include "Common\Utility\kpuMatrix.h"

class kpgGeometry;

class kpgGeometryInstance
{
public:
	kpgGeometryInstance(kpgGeometry* pGeometry);
	kpgGeometryInstance(kpgGeometry* pGeometry, const kpuMatrix& mLocalToWorld);
	virtual ~kpgGeometryInstance(void);
					
	void SetRotation(float fX, float fY, float fZ)		{ m_mLocalToWorld.SetAxisRotation(fX, fY, fZ); }
	void SetPosition(float fX, float fY, float fZ)		{ m_mLocalToWorld.SetD(kpuVector(fX, fY, fZ, 1.0f)); }
	void SetPermYRotation(float fY)						{ m_mLocalToWorld.SetRotationY(fY); }

	kpgGeometry* GetGeometry()							{ return m_pGeometry; }
	const kpuMatrix& GetMatrix()						    { return m_mLocalToWorld; }
	void SetMatrix(const kpuMatrix& matrix)				{ m_mLocalToWorld = matrix; }

protected:
	kpgGeometry*		m_pGeometry;
	kpuMatrix			m_mLocalToWorld;
};
