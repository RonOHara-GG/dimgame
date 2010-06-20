#pragma once

#include <d3d9.h>
#include <dinput.h>
#include "Common/Utility/kpuVector.h"

enum eInputEventType
{
	eIET_ButtonDown,
	eIET_ButtonUp,
	eIET_ButtonRepeat,
};

class kpiInputManager
{
public:

	kpiInputManager( HWND window );
	~kpiInputManager(void);

	bool Update();

	kpuVector	MousePos();

protected:
	HWND					m_hWnd;
	IDirectInput8*			m_pDirectInput;
	ul32					m_ulPressStamp;

	//keyboard input
	IDirectInputDevice8*	m_pKeyboard;	
	char					m_acKeyState[256];
	ul32					m_aulKeyPressStamp[256];

	//Mouse input
	IDirectInputDevice8*	m_pMouse;	
	DIMOUSESTATE			m_MouseState;
	ul32					m_ulButtonPressStamp[3];
	kpuVector				m_vMousePos;

	HANDLE					m_ahEvents[2]; //Input events
};

extern void	InputEvent(eInputEventType type, u32 button);

#define KPI_MOUSE_BTN_0		0x01
#define KPI_MOUSE_BTN_1		0x02

