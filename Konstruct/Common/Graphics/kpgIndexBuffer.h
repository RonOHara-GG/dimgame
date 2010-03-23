#pragma once

#include <d3d9.h>

class kpgIndexBuffer
{
public:
	kpgIndexBuffer(void);
	~kpgIndexBuffer(void);

	bool Create(int nIndexCount);

	bool Lock();
	bool Unlock();

	void SetIndex(int nIndex, unsigned short nValue);
	int GetIndex(int nIndex) const;

	int GetIndexCount()					{ return m_nIndexCount; }
	operator IDirect3DIndexBuffer9*()	{ Unlock(); return m_pIndexBuffer; }

protected:
	int						m_nIndexCount;
	IDirect3DIndexBuffer9*	m_pIndexBuffer;
	unsigned short*			m_pLockPtr;
	bool					m_bLocked;
};
