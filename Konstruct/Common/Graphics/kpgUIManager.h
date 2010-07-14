#pragma once
#include "Common/Utility/kpuMatrix.h"
#include "Common/Utility/kpuMap.h"

class kpgRenderer;
class kpgUIWindow;
class kpgUITextInput;
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
	void ToggleUIWindow(u32 uHash);
	void OpenUIWindow(u32 uHash);
	void CloseUIWindow(u32 uHash);

	void SetDataSource(char* pszName, char* pData);
	char** GetDataSource(char* pszName);
	kpgUIWindow* GetUIWindow(u32 uHash);

protected:	
	kpuMatrix							m_mUIRenderMatrix;
	kpgUIWindow*						m_pCurrentWindow;
	kpgUIWindow*						m_pWinMouseOver;
	kpuLinkedList*						m_plWindowList;	

	kpgUITextInput*						m_pCurrentInput;
	kpuMap<char*,char*>*				m_pDataSourceMap;
	
};

/*Window Hashed Click Events*/
#define IE_NOT_HANDLED	0x01
#define CE_NEW_WINDOW	0x40ed18e7	//Move to a new window
#define CE_OPEN			0x7c8a4b57 //Set a window to visible
#define CE_CLOSE		0xcf88a3b //Set a window to invisible
#define CE_SET_INPUT	0x0 //Set the current text input
#define CE_SCROLL_UP	0xebd294f9
#define CE_SCROLL_DOWN	0x2ac2704c
#define CE_SELECT_CELL	0x73fb9385