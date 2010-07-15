#pragma once
#include "Common/Utility/kpuMatrix.h"
#include "Common/Utility/kpuMap.h"

class kpgRenderer;
class kpgUIWindow;
class kpgUITextInput;
class kpuLinkedList;
class kpuPhysicalObject;
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
	void ToggleUIWindow(u32 uHash);
	void OpenUIWindow(u32 uHash, kpuPhysicalObject* pContext = 0);
	void CloseUIWindow(u32 uHash);
	void CloseAll();
	void SetWinMouseOver(kpgUIWindow* pWindow) { m_pWinMouseOver = pWindow; }
	kpgUIWindow* WinMouseOver() { return m_pWinMouseOver; }
	

	void SetDataSource(char* pszName, char* pData);
	char** GetDataSource(char* pszName);
	kpgUIWindow* GetUIWindow(u32 uHash);

protected:	
	kpuMatrix							m_mUIRenderMatrix;
	kpuLinkedList						m_lCurrentWindow;
	kpgUIWindow*						m_pWinMouseOver;
	kpuLinkedList*						m_plWindowList;	

	kpgUITextInput*						m_pCurrentInput;
	kpuMap<char*,char*>*				m_pDataSourceMap;
	
};

/*Window Hashed Click Events*/
#define IE_NOT_HANDLED	0x01
