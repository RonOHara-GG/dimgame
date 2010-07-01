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
	void NewWindow(u32 uHash);

protected:	
	kpgUIWindow* GetUIWindow(u32 uHash);
	

	kpuMatrix			m_mUIRenderMatrix;
	kpgUIWindow*		m_pCurrentWindow;
	kpgUIWindow*		m_pWinMouseOver;
	kpuLinkedList*		m_plWindowList;	
};

/*Window Hashed Click Events*/
#define IE_NOT_HANDLED	0x01
#define CE_NEW_WINDOW	0x40ed18e7	//Move to a new window
#define CE_SHOW			0x7c8c5c26 //Set a window to visible
#define CE_CLOSE		0xcf88a3b //Set a window to invisible