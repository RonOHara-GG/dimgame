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

	u32 HandleInputEvent(eInputEventType type, u32 button); //return unhandled event, 0 if events are handled

	bool LoadWindows(const char* szFile);
	kpgUIWindow* GetUIWindow(u32 uHash);

protected:	
	kpuMatrix			m_mUIRenderMatrix;
	kpgUIWindow*		m_pCurrentWindow;
	kpuLinkedList*		m_plWindowList;	
};

/*Window Hashed Click Events*/
#define IE_NOT_HANDLED	0x01
#define CE_NEW_WINDOW	0x40ed18e7	//Move to a new window