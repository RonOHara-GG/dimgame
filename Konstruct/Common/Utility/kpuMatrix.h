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
	void SetRotationAboutAxis(const kpuVector& vAxis, float fRadians);
	void SetScale(float fScaleX, float fScaleY, float fScaleZ);

	kpuMatrix	operator *(float fVal) const;
	kpuMatrix	operator *(const kpuMatrix& m) const;
	kpuMatrix	operator -(const kpuMatrix& m) const;
	kpuMatrix	operator +(const kpuMatrix& m) const;
	bool		operator !=(const kpuMatrix& m);

	void Zero();
	void Identity();
	void Invert();
	void Transpose();
	void Transform(const kpuMatrix& m);
	void RotateLocal(const kpuMatrix& m);			// Rotate this matrix by the other matrix (does not rotate the translation)

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

inline void kpuMatrix::SetRotationAboutAxis(const kpuVector& vAxis, float fRadians)
{
	// borrowed from http://www.cprogramming.com/tutorial/3d/rotation.html

	float fSin = sinf(fRadians);
	float fCos = cosf(fRadians);
	float fT = 1.0f - fCos;

	m_vA.Set((fT * vAxis.GetX() * vAxis.GetX()) + fCos,						(fT * vAxis.GetX() * vAxis.GetY()) + (fSin * vAxis.GetZ()),	(fT * vAxis.GetX() * vAxis.GetZ()) - (fSin * vAxis.GetY()), 0.0f);
	m_vB.Set((fT * vAxis.GetX() * vAxis.GetY()) - (fSin * vAxis.GetZ()),	(fT * vAxis.GetY() * vAxis.GetY()) + fCos,					(fT * vAxis.GetY() * vAxis.GetZ()) + (fSin * vAxis.GetX()), 0.0f);
	m_vC.Set((fT * vAxis.GetX() * vAxis.GetY()) + (fSin * vAxis.GetY()),	(fT * vAxis.GetY() * vAxis.GetZ()) - (fSin * vAxis.GetX()), (fT * vAxis.GetZ() * vAxis.GetZ()) + fCos,					0.0f);
	m_vD = kpuv_OneW;
}

inline void kpuMatrix::SetScale(float fScaleX, float fScaleY, float fScaleZ)
{
	m_vA.Set(fScaleX, 0.0f, 0.0f, 0.0f);
	m_vB.Set(0.0f, fScaleY, 0.0f, 0.0f);
	m_vC.Set(0.0f, 0.0f, fScaleZ, 0.0f);
	m_vD = kpuv_OneW;
}

inline kpuMatrix kpuMatrix::operator *(float fVal) const
{
	kpuMatrix ret;

	kpuVector v = m_vA * fVal;
	ret.SetA(v);
	v = m_vB * fVal;
	ret.SetB(v);
	v = m_vC * fVal;
	ret.SetC(v);
	v = m_vD * fVal;
	ret.SetD(v);

	return ret;
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

inline kpuMatrix kpuMatrix::operator -(const kpuMatrix& m) const
{
	kpuMatrix ret;

	float fX, fY, fZ, fW;
	fX = m_vA.GetX() - m.m_vA.GetX();
	fY = m_vA.GetY() - m.m_vA.GetY();
	fZ = m_vA.GetZ() - m.m_vA.GetZ();
	fW = m_vA.GetW() - m.m_vA.GetW();
	ret.SetA(kpuVector(fX, fY, fZ, fW));

	fX = m_vB.GetX() - m.m_vB.GetX();
	fY = m_vB.GetY() - m.m_vB.GetY();
	fZ = m_vB.GetZ() - m.m_vB.GetZ();
	fW = m_vB.GetW() - m.m_vB.GetW();
	ret.SetB(kpuVector(fX, fY, fZ, fW));

	fX = m_vC.GetX() - m.m_vC.GetX();
	fY = m_vC.GetY() - m.m_vC.GetY();
	fZ = m_vC.GetZ() - m.m_vC.GetZ();
	fW = m_vC.GetW() - m.m_vC.GetW();
	ret.SetC(kpuVector(fX, fY, fZ, fW));

	fX = m_vD.GetX() - m.m_vD.GetX();
	fY = m_vD.GetY() - m.m_vD.GetY();
	fZ = m_vD.GetZ() - m.m_vD.GetZ();
	fW = m_vD.GetW() - m.m_vD.GetW();
	ret.SetD(kpuVector(fX, fY, fZ, fW));

	return ret;
}

inline kpuMatrix kpuMatrix::operator +(const kpuMatrix& m) const
{
	kpuMatrix ret;

	float fX, fY, fZ, fW;
	fX = m_vA.GetX() + m.m_vA.GetX();
	fY = m_vA.GetY() + m.m_vA.GetY();
	fZ = m_vA.GetZ() + m.m_vA.GetZ();
	fW = m_vA.GetW() + m.m_vA.GetW();
	ret.SetA(kpuVector(fX, fY, fZ, fW));

	fX = m_vB.GetX() + m.m_vB.GetX();
	fY = m_vB.GetY() + m.m_vB.GetY();
	fZ = m_vB.GetZ() + m.m_vB.GetZ();
	fW = m_vB.GetW() + m.m_vB.GetW();
	ret.SetB(kpuVector(fX, fY, fZ, fW));

	fX = m_vC.GetX() + m.m_vC.GetX();
	fY = m_vC.GetY() + m.m_vC.GetY();
	fZ = m_vC.GetZ() + m.m_vC.GetZ();
	fW = m_vC.GetW() + m.m_vC.GetW();
	ret.SetC(kpuVector(fX, fY, fZ, fW));

	fX = m_vD.GetX() + m.m_vD.GetX();
	fY = m_vD.GetY() + m.m_vD.GetY();
	fZ = m_vD.GetZ() + m.m_vD.GetZ();
	fW = m_vD.GetW() + m.m_vD.GetW();
	ret.SetD(kpuVector(fX, fY, fZ, fW));

	return ret;
}

inline bool kpuMatrix::operator !=(const kpuMatrix& m)
{
	if( m_vA != m.m_vA )
		return true;
	if( m_vB != m.m_vB )
		return true;
	if( m_vC != m.m_vC )
		return true;
	if( m_vD != m.m_vD )
		return true;

	return false;
}

inline void kpuMatrix::RotateLocal(const kpuMatrix& m)
{
	kpuMatrix trans = m;
	trans.Transpose();

	float fX, fY, fZ;
	fX = m_vA.Dot(trans.m_vA);
	fY = m_vA.Dot(trans.m_vB);
	fZ = m_vA.Dot(trans.m_vC);
	m_vA.SetX(fX);
	m_vA.SetY(fY);
	m_vA.SetZ(fZ);

	fX = m_vB.Dot(trans.m_vA);
	fY = m_vB.Dot(trans.m_vB);
	fZ = m_vB.Dot(trans.m_vC);
	m_vB.SetX(fX);
	m_vB.SetY(fY);
	m_vB.SetZ(fZ);

	fX = m_vC.Dot(trans.m_vA);
	fY = m_vC.Dot(trans.m_vB);
	fZ = m_vC.Dot(trans.m_vC);
	m_vC.SetX(fX);
	m_vC.SetY(fY);
	m_vC.SetZ(fZ);
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

inline kpuVector kpuVector::operator *(const kpuMatrix& m)
{
	kpuMatrix trans = m;
	trans.Transpose();	

	float fX = Dot4(trans.m_vA);
	float fY = Dot4(trans.m_vB);
	float fZ = Dot4(trans.m_vC);
	float fW = Dot4(trans.m_vD);

	return kpuVector(fX, fY, fZ, fW);
}