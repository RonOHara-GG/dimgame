#pragma once
#include "Common/Utility/kpuMatrix.h"

class kpgRenderer;
class kpgUIWindow;
class kpuLinkedList;
enum eInputEventType;

class kpgUIManager
{
public:
	kpgUIManager(void);
	~kpgUIManager(void);

	void Update();
	void Draw(kpgRenderer* pRenderer);

	bool HandleInputEvent(eInputEventType type, u32 button);

	bool LoadWindows(const char* szFile);
	kpgUIWindow* GetUIWindow(u32 uHash);

protected:	
	kpuMatrix			m_mUIRenderMatrix;
	kpgUIWindow*		m_pCurrentWindow;
	kpuLinkedList*		m_plWindowList;	
};
