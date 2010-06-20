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
	m_ahEvents[0] = CreateEvent(NULL, false, false, NULL);
	m_pKeyboard->SetEventNotification(m_ahEvents[0]);	
	m_pKeyboard->Acquire();
	
	//get mouse
	m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);	
	m_ahEvents[1] = CreateEvent(NULL, false, false, NULL);
	m_pMouse->SetEventNotification(m_ahEvents[1]);
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
	DWORD dwResult = WaitForMultipleObjects(2,m_ahEvents, false, 0);

	switch( dwResult )
	{
	case WAIT_OBJECT_0:
		{
			// key board update
			if( SUCCEEDED( m_pKeyboard->GetDeviceState(sizeof(m_acKeyState), (LPVOID)m_acKeyState)) )
			{
				//check keys


			}
			break;
		}
	case WAIT_OBJECT_0 + 1:
		{
			//mouse update
			if( SUCCEEDED( m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState)) )
			{
				POINT mousePos;
				GetCursorPos(&mousePos);
				ScreenToClient(m_hWnd, &mousePos);
				m_vMousePos.SetX(mousePos.x);
				m_vMousePos.SetY(mousePos.y);

				//check buttons
				

				

			}
			break;
		}
	}	

	// Update joypad states

	return true;
}