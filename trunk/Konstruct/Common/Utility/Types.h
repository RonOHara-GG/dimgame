#pragma once

typedef char				s8;
typedef unsigned char		u8;
typedef short				s16;
typedef unsigned short		u16;
typedef int					s32;
typedef unsigned int		u32;
typedef unsigned long		ul32;
typedef __int64				s64;
typedef unsigned __int64	u64;


typedef struct _kpRect
{
	float	m_fLeft;
	float	m_fRight;
	float	m_fTop;
	float	m_fBottom;

	_kpRect() {}
	_kpRect(float fL, float fR, float fT, float fB) : m_fLeft(fL), m_fRight(fR), m_fTop(fT), m_fBottom(fB) {}

	void Shrink(float fUnits = 1.0f)				{ m_fLeft += fUnits; m_fTop += fUnits; m_fRight -= fUnits; m_fBottom -= fUnits; }
	bool PointInside(float fX, float fY) const		{ return (fX >= m_fLeft && fX <= m_fRight && fY >= m_fTop && fY <= m_fBottom); }
	void TransformPoint(float& fX, float& fY) const	{ fX = TransformX(fX); fY = TransformY(fY); }
	float TransformX(float fVal) const				{ float fWidth = m_fRight - m_fLeft; float fLen = fVal - m_fLeft; return fLen / fWidth; }
	float TransformY(float fVal) const				{ float fHeight = m_fBottom - m_fTop; float fLen = fVal - m_fTop; return fLen / fHeight; }
	float Width() const								{ return m_fRight - m_fLeft; }
	float Height() const							{ return m_fBottom - m_fTop; }
} kpRect;

#define kpuSWAP(a, b, c)	\
{							\
	c = a;					\
	a = b;					\
	b = c;					\
}

#define PI				3.1415926535897f
#define RAD_TO_DEG		57.295779513082f
#define DEG_TO_RAD		0.0174532925199f

#define MAX(x, y)	((x > y) ? x : y)
#define MIN(x, y)	((x < y) ? x : y)

void Printf(const char* pFormat, ...);

u32 StringHash(const char* InputString);
u32 StringHash(const char* InputString, const char* IgnoreCharacters);