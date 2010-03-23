#include "StdAfx.h"
#include "Common\Utility\kpuMatrix.h"
#include <math.h>
#include <d3dx9math.h>

kpuMatrix::kpuMatrix(void)
{
}

kpuMatrix::~kpuMatrix(void)
{
}

void kpuMatrix::Perspective(float fFoVDegreesY, float fAspectRatio, float fZNear, float fZFar)
{
//	Zero();
	
//	float fYScale = 1.0f / tanf((fFoVDegreesY * DEG_TO_RAD) * 0.5f);
//	m_vA.SetX(fAspectRatio * fYScale);
//	m_vB.SetY(fYScale);
//	m_vC.SetZ(fZFar / (fZFar - fZNear));
//	m_vC.SetW(1.0f);
//	m_vD.SetZ(-fZNear * fZFar / (fZFar - fZNear));

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovRH( &matProj, fFoVDegreesY * DEG_TO_RAD, fAspectRatio, fZNear, fZFar );

	memcpy(this, &matProj, sizeof(kpuMatrix));
}

void kpuMatrix::Orthographic(float fWidth, float fHeight, float fZNear, float fZFar)
{
	//Zero();
	//m_vA.SetX(2.0f / fWidth);
	//m_vB.SetY(2.0f / fHeight);
	//m_vC.SetZ(1.0f / (fZFar - fZNear));
	//m_vD.SetZ(fZNear / (fZNear - fZFar));
	//m_vD.SetW(1.0f);

	D3DXMATRIX matProj;
	//D3DXMatrixOrthoLH(&matProj, fWidth, fHeight, fZNear, fZFar);
	D3DXMatrixOrthoOffCenterRH(&matProj, 0.0f, fWidth, fHeight, 0.0f, fZNear, fZFar);

	memcpy(this, &matProj, sizeof(kpuMatrix));
}

void kpuMatrix::LookAt(const kpuVector& vPosition, const kpuVector& vTarget, const kpuVector& vUp)
{
	//kpuVector vViewDir = vTarget - vPosition;
	//vViewDir.Normalize();
	//kpuVector vRight = vUp % vViewDir;
	//vRight.Normalize();
	//kpuVector vRealUp = vViewDir % vRight;

	//m_vA.Set(vRight.GetX(), vRealUp.GetX(), vViewDir.GetX(), 0.0f);
	//m_vB.Set(vRight.GetY(), vRealUp.GetY(), vViewDir.GetY(), 0.0f);
	//m_vC.Set(vRight.GetZ(), vRealUp.GetZ(), vViewDir.GetZ(), 0.0f);
	//m_vD = vPosition;
	//m_vD.SetW(1.0f);

	D3DXMATRIX matView;
	D3DXMatrixLookAtRH( &matView, (const D3DXVECTOR3*)&vPosition, (const D3DXVECTOR3*)&vTarget, (const D3DXVECTOR3*)&vUp );
	
	memcpy(this, &matView, sizeof(kpuMatrix));
}

void kpuMatrix::Invert()
{
	D3DXMATRIX mCopy = *(D3DXMATRIX*)&m_vA;
	D3DXMATRIX mInv;
	if( D3DXMatrixInverse(&mInv, NULL, &mCopy) )
	{
		*(D3DXMATRIX*)&m_vA = mInv;
	}
}