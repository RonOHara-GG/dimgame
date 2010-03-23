#pragma once

#include "Common\Utility\kpuVector.h"

class kpuMatrix
{
	friend class kpuVector;

public:
	kpuMatrix(void);
	kpuMatrix(const kpuVector& a, const kpuVector& b, const kpuVector& c, const kpuVector& d);
	~kpuMatrix(void);

	kpuVector GetA() const;
	kpuVector GetB() const;
	kpuVector GetC() const;
	kpuVector GetD() const;

	void SetA(const kpuVector& a);
	void SetB(const kpuVector& b);
	void SetC(const kpuVector& c);
	void SetD(const kpuVector& d);
	void SetAxisRotation(float fX, float fY, float fZ);
	void SetRotationX(float fXRadians);
	void SetRotationY(float fYRadians);
	void SetRotationZ(float fZRadians);

	kpuMatrix	operator *(const kpuMatrix& m) const;

	void Zero();
	void Identity();
	void Invert();
	void Transpose();
	void Transform(const kpuMatrix& m);

	void Perspective(float fFoVDegreesY, float fAspectRatio, float fZNear, float fZFar);
	void Orthographic(float fWidth, float fHeight, float fZNear, float fZFar);
	void LookAt(const kpuVector& vPosition, const kpuVector& vTarget, const kpuVector& vUp);

protected:
	kpuVector	m_vA;
	kpuVector	m_vB;
	kpuVector	m_vC;
	kpuVector	m_vD;
};

// Implementations

inline kpuMatrix::kpuMatrix(const kpuVector& a, const kpuVector& b, const kpuVector& c, const kpuVector& d) :
	m_vA(a), m_vB(b), m_vC(c), m_vD(d)
{
}

inline kpuVector kpuMatrix::GetA() const
{
	return m_vA;
}

inline kpuVector kpuMatrix::GetB() const
{
	return m_vB;
}

inline kpuVector kpuMatrix::GetC() const
{
	return m_vC;
}

inline kpuVector kpuMatrix::GetD() const
{
	return m_vD;
}

inline void kpuMatrix::SetA(const kpuVector& a)
{
	m_vA = a;
}

inline void kpuMatrix::SetB(const kpuVector& b)
{
	m_vB = b;
}

inline void kpuMatrix::SetC(const kpuVector& c)
{
	m_vC = c;
}

inline void kpuMatrix::SetD(const kpuVector& d)
{
	m_vD = d;
}

inline void kpuMatrix::SetAxisRotation(float fX, float fY, float fZ)
{
	// Set the rotation/scale section of the matrix to identity
	m_vA = kpuv_OneX;
	m_vB = kpuv_OneY;
	m_vC = kpuv_OneZ;

	// Build X Rotation
	kpuMatrix mXRot;
	mXRot.SetRotationX(fX);

	// Build Y Rotation
	kpuMatrix mYRot;
	mYRot.SetRotationY(fY);

	// Build Z Rotation
	kpuMatrix mZRot;
	mZRot.SetRotationZ(fZ);

	// Rotate!
	kpuVector vTemp = m_vD;
	m_vD = kpuv_OneW;
	Transform(mXRot * mYRot * mZRot);
	m_vD = vTemp;
}

inline void kpuMatrix::SetRotationX(float fXRadians)
{
	float fSin = sinf(fXRadians);
	float fCos = cosf(fXRadians);
	m_vA = kpuv_OneX;
	m_vB.Set(0.0f, fCos, fSin, 0.0f);
	m_vC.Set(0.0f, -fSin, fCos, 0.0f);
	m_vD = kpuv_OneW;
}

inline void kpuMatrix::SetRotationY(float fYRadians)
{
	float fSin = sinf(fYRadians);
	float fCos = cosf(fYRadians);
	m_vA.Set(fCos, 0.0f, -fSin, 0.0f);
	m_vB = kpuv_OneY;
	m_vC.Set(fSin, 0.0f, fCos, 0.0f);
	m_vD = kpuv_OneW;
}

inline void kpuMatrix::SetRotationZ(float fZRadians)
{
	float fSin = sinf(fZRadians);
	float fCos = cosf(fZRadians);
	m_vA.Set(fCos, fSin, 0.0f, 0.0f);
	m_vB.Set(-fSin, fCos, 0.0f, 0.0f);
	m_vC = kpuv_OneZ;
	m_vD = kpuv_OneW;
}

inline kpuMatrix kpuMatrix::operator *(const kpuMatrix& m) const
{
	kpuMatrix ret;
	kpuMatrix trans = m;
	trans.Transpose();

	float fX, fY, fZ, fW;
	fX = m_vA.Dot4(trans.m_vA);
	fY = m_vA.Dot4(trans.m_vB);
	fZ = m_vA.Dot4(trans.m_vC);
	fW = m_vA.Dot4(trans.m_vD);
	ret.SetA(kpuVector(fX, fY, fZ, fW));

	fX = m_vB.Dot4(trans.m_vA);
	fY = m_vB.Dot4(trans.m_vB);
	fZ = m_vB.Dot4(trans.m_vC);
	fW = m_vB.Dot4(trans.m_vD);
	ret.SetB(kpuVector(fX, fY, fZ, fW));

	fX = m_vC.Dot4(trans.m_vA);
	fY = m_vC.Dot4(trans.m_vB);
	fZ = m_vC.Dot4(trans.m_vC);
	fW = m_vC.Dot4(trans.m_vD);
	ret.SetC(kpuVector(fX, fY, fZ, fW));

	fX = m_vD.Dot4(trans.m_vA);
	fY = m_vD.Dot4(trans.m_vB);
	fZ = m_vD.Dot4(trans.m_vC);
	fW = m_vD.Dot4(trans.m_vD);
	ret.SetD(kpuVector(fX, fY, fZ, fW));

	return ret;
}

inline void kpuMatrix::Zero()
{
	memset(&m_vA, 0, sizeof(kpuMatrix));
}

inline void kpuMatrix::Identity()
{
	Zero();
	m_vA.SetX(1.0f);
	m_vB.SetY(1.0f);
	m_vC.SetZ(1.0f);
	m_vD.SetW(1.0f);
}

inline void kpuMatrix::Transpose()
{
	float fTemp;
	float* pFloats = (float*)&m_vA;
	
	kpuSWAP(pFloats[1], pFloats[4], fTemp);
	kpuSWAP(pFloats[2], pFloats[8], fTemp);
	kpuSWAP(pFloats[3], pFloats[12], fTemp);
	kpuSWAP(pFloats[6], pFloats[9], fTemp);
	kpuSWAP(pFloats[7], pFloats[13], fTemp);
	kpuSWAP(pFloats[11], pFloats[14], fTemp);
}

inline void kpuMatrix::Transform(const kpuMatrix& m)
{
	kpuMatrix result = m * *this;
	*this = result;
}

// Vector implementations
inline void kpuVector::operator *=(const kpuMatrix& m)
{
	kpuMatrix trans = m;
	trans.Transpose();	

	float fX = Dot4(trans.m_vA);
	float fY = Dot4(trans.m_vB);
	float fZ = Dot4(trans.m_vC);
	float fW = Dot4(trans.m_vD);

	Set(fX, fY, fZ, fW);
}