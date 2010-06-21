#include "StdAfx.h"
#include "kpiInputManager.h"

kpiInputManager::kpiInputManager( HWND window )
{
	m_hWnd= window;

	//create device
	DirectInput8Create(GetModuleHandle( NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);

	//get keyboard
	m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL);
	m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyboard->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);	
	m_pKeyboard->Acquire();
	
	//get mouse
	m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);	
	m_pMouse->Acquire();
	

	m_ulPressStamp = 0;	
}

kpiInputManager::~kpiInputManager(void)
{
	m_pKeyboard->Unacquire();
	m_pMouse->Unacquire();
	m_pDirectInput->Release();
}

bool kpiInputManager::Update()
{
	//Update keyboard states
	BYTE	newKeyState[256];
	if( SUCCEEDED(m_pKeyboard->GetDeviceState(sizeof(newKeyState), &newKeyboardState)) )
	{
		//check new input against old
		for(int i = 0; i < 256; i++ )
		{
			if( m_aKeyState[i] != newKeyBoardState[i] )
			{
				if( m_aKeyState[i] != 0 )
					InputEvent(eIET_ButtonUp, m_aKeyState[i]);

				if( newKeyState[i] != 0 )
					InputEvent(eIET_ButtonDown, newKeyState[i]);
			}
		}

		if( FAILED(m_pKeyboard->Acquire() )
			return false;

	}


	//Update mouse states

	// Update joypad states

	return true;
}