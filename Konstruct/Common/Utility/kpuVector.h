#pragma once

#include <math.h>

class kpuMatrix;

class kpuVector
{
public:
	kpuVector(void);
	kpuVector(float fX, float fY, float fZ, float fW);
	~kpuVector(void);

	float GetX() const;
	float GetY() const;
	float GetZ() const;
	float GetW() const;

	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);
	void SetW(float w);
	void Set(float x, float y, float z, float w);

	kpuVector	operator +(const kpuVector& v) const;		// Add
	kpuVector	operator -(const kpuVector& v) const;		// Subtract
	kpuVector	operator -() const;							// Negate
	kpuVector	operator *(const kpuVector& v) const;		// Multiply
	kpuVector	operator *(float fScalar) const;			// Scale
	kpuVector	operator /(const kpuVector& v) const;		// Divide
	kpuVector	operator %(const kpuVector& v) const;		// Cross
	void     	operator =(const kpuVector& v);				//Equals

	void		operator +=(const kpuVector& v);			// Add
	void		operator -=(const kpuVector& v);			// Subtract
	void		operator *=(const kpuVector& v);			// Multiply
	void		operator *=(float fScalar);					// Scale
	void		operator /=(const kpuVector& v);			// Divide

	void		operator *=(const kpuMatrix& m);

	bool		operator ==(const kpuVector& v) const;		// Strict binary equals
	bool		operator !=(const kpuVector& v) const;

	float Dot(const kpuVector& v) const;
	float Dot4(const kpuVector& v) const;
	float Length() const;

	static	kpuVector		 Normalize(const kpuVector &v);
	static	float			 DistanceSquared(const kpuVector& v1, const kpuVector& v2);

	void Normalize();

	void RotateX(float fRadians);
	void RotateY(float fRadians);
	void RotateZ(float fRadians);

	void Lerp(const kpuVector& vA, const kpuVector& vB, float fT);

	void Truncate(float fMax); //If teh vector's magnitude is greater than max it is made equal

protected:
	float	m_fX;
	float	m_fY;
	float	m_fZ;
	float	m_fW;
};

// Static pre-setup vectors
extern const kpuVector	kpuv_Zero;
extern const kpuVector	kpuv_OneX;
extern const kpuVector	kpuv_OneY;
extern const kpuVector	kpuv_OneZ;
extern const kpuVector	kpuv_OneW;

// Implementations
inline kpuVector::kpuVector(float fX, float fY, float fZ, float fW) : m_fX(fX), m_fY(fY), m_fZ(fZ), m_fW(fW)
{
}

inline float kpuVector::GetX() const
{
	return m_fX;
}

inline float kpuVector::GetY() const
{
	return m_fY;
}

inline float kpuVector::GetZ() const
{
	return m_fZ;
}

inline float kpuVector::GetW() const
{
	return m_fW;
}

inline void kpuVector::SetX(float x)
{
	m_fX = x;
}

inline void kpuVector::SetY(float y)
{
	m_fY = y;
}

inline void kpuVector::SetZ(float z)
{
	m_fZ = z;
}

inline void kpuVector::SetW(float w)
{
	m_fW = w;
}

inline void kpuVector::Set(float x, float y, float z, float w)
{
	m_fX = x;
	m_fY = y;
	m_fZ = z;
	m_fW = w;
}

inline kpuVector kpuVector::operator +(const kpuVector& v) const
{
	return kpuVector(m_fX + v.m_fX, m_fY + v.m_fY, m_fZ + v.m_fZ, m_fW + v.m_fW);
}

inline kpuVector kpuVector::operator -(const kpuVector& v) const
{
	return kpuVector(m_fX - v.m_fX, m_fY - v.m_fY, m_fZ - v.m_fZ, m_fW - v.m_fW);
}

inline kpuVector kpuVector::operator -() const
{
	return kpuVector(-m_fX, -m_fY, -m_fZ, -m_fW);
}

inline kpuVector kpuVector::operator *(const kpuVector& v) const
{
	return kpuVector(m_fX * v.m_fX, m_fY * v.m_fY, m_fZ * v.m_fZ, m_fW * v.m_fW);
}

inline kpuVector kpuVector::operator *(float fScalar) const
{
	return kpuVector(m_fX * fScalar, m_fY * fScalar, m_fZ * fScalar, m_fW * fScalar);
}

inline kpuVector kpuVector::operator /(const kpuVector& v) const
{
	return kpuVector(m_fX / v.m_fX, m_fY / v.m_fY, m_fZ / v.m_fZ, m_fW / v.m_fW);
}

inline kpuVector kpuVector::operator %(const kpuVector& v) const
{
	return kpuVector((m_fY * v.m_fZ) - (m_fZ * v.m_fY), (m_fZ * v.m_fX) - (m_fX * v.m_fZ), (m_fX * v.m_fY) - (m_fY * v.m_fX), 0.0f);
}

inline void kpuVector::operator =(const kpuVector& v)
{
	m_fX = v.m_fX;
	m_fY = v.m_fY;
	m_fZ = v.m_fZ;
	m_fW = v.m_fW;
}

inline void kpuVector::operator +=(const kpuVector& v)
{
	m_fX += v.m_fX;
	m_fY += v.m_fY;
	m_fZ += v.m_fZ;
	m_fW += v.m_fW;
}

inline void kpuVector::operator -=(const kpuVector& v)
{
	m_fX -= v.m_fX;
	m_fY -= v.m_fY;
	m_fZ -= v.m_fZ;
	m_fW -= v.m_fW;
}

inline void kpuVector::operator *=(const kpuVector& v)
{
	m_fX *= v.m_fX;
	m_fY *= v.m_fY;
	m_fZ *= v.m_fZ;
	m_fW *= v.m_fW;
}

inline void kpuVector::operator *=(float fScalar)
{
	m_fX *= fScalar;
	m_fY *= fScalar;
	m_fZ *= fScalar;
	m_fW *= fScalar;
}

inline void kpuVector::operator /=(const kpuVector& v)
{
	m_fX /= v.m_fX;
	m_fY /= v.m_fY;
	m_fZ /= v.m_fZ;
	m_fW /= v.m_fW;
}

inline bool kpuVector::operator ==(const kpuVector& v) const
{
	return( m_fX == v.m_fX && m_fY == v.m_fY && m_fZ == v.m_fZ && m_fW == v.m_fW );
}

inline bool kpuVector::operator !=(const kpuVector& v) const
{
	return( m_fX != v.m_fX || m_fY != v.m_fY || m_fZ != v.m_fZ || m_fW != v.m_fW );
}

inline float kpuVector::Dot(const kpuVector& v) const
{
	return ((m_fX * v.m_fX) + (m_fY * v.m_fY) + (m_fZ * v.m_fZ));
}

inline float kpuVector::Dot4(const kpuVector& v) const
{
	return ((m_fX * v.m_fX) + (m_fY * v.m_fY) + (m_fZ * v.m_fZ) + (m_fW * v.m_fW));
}

inline float kpuVector::Length() const
{
	return sqrtf(Dot(*this));
}

inline void kpuVector::Normalize()
{
	float fRecip = 1.0f / Length();
	m_fX *= fRecip;
	m_fY *= fRecip;
	m_fZ *= fRecip;
	m_fW *= fRecip;
}

inline kpuVector kpuVector::Normalize(const kpuVector &v)
{
	kpuVector vReturn = v;
	float fRecip = 1.0f / v.Length();
	vReturn.m_fX *= fRecip;
	vReturn.m_fY *= fRecip;
	vReturn.m_fZ *= fRecip;
	vReturn.m_fW *= fRecip;

	return vReturn;
}

inline void kpuVector::RotateX(float fRadians)
{
	float s = sinf(fRadians);
	float c = cosf(fRadians);

	float y = (m_fY * c) - (m_fZ * s);
	float z = (m_fY * s) + (m_fZ * c);

	m_fY = y;
	m_fZ = z;
}

inline void kpuVector::RotateY(float fRadians)
{
	float s = sinf(fRadians);
	float c = cosf(fRadians);
	
	float x = (m_fZ * s) + (m_fX * c);
	float z = (m_fZ * c) - (m_fX * s);

	m_fX = x;
	m_fZ = z;
}

inline void kpuVector::RotateZ(float fRadians)
{
	float s = sinf(fRadians);
	float c = cosf(fRadians);

	float x = (m_fX * c) - (m_fY * s);
	float y = (m_fX * s) + (m_fY * c);

	m_fX = x;
	m_fY = y;
}

inline void kpuVector::Lerp(const kpuVector& vA, const kpuVector& vB, float fT)
{
	kpuVector vAtoB = vB - vA;
	m_fX = vA.m_fX + (vAtoB.m_fX * fT);
	m_fY = vA.m_fY + (vAtoB.m_fY * fT);
	m_fZ = vA.m_fZ + (vAtoB.m_fZ * fT);
	m_fW = vA.m_fW + (vAtoB.m_fW * fT);
}

inline void kpuVector::Truncate(float fMax)
{
	float fLengthSqrd = Dot(*this);

	if(fLengthSqrd > fMax* fMax)
	{
		//Make them equal
		m_fX /= (fLengthSqrd / (fMax* fMax));
		m_fY /= (fLengthSqrd / (fMax* fMax));
		m_fZ /= (fLengthSqrd / (fMax* fMax));
	}
}

inline float kpuVector::DistanceSquared(const kpuVector &v1, const kpuVector &v2)
{
	return ((v2.m_fX - v1.m_fX) * (v2.m_fX - v1.m_fX)) +
			((v2.m_fY - v1.m_fY) * (v2.m_fY - v1.m_fY)) +
			((v2.m_fZ - v1.m_fZ) * (v2.m_fZ - v1.m_fZ));


}