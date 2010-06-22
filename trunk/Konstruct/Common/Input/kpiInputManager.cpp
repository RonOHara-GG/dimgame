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
	m_pKeyboard->GetDeviceState(sizeof(m_aKeyState), &m_aKeyState);
	
	//get mouse
	m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);	
	m_pMouse->Acquire();
	m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);

	m_iaButtonClickStamp[0] = -1;
	m_iaButtonClickStamp[1] = -1;
	m_iaButtonClickStamp[2] = -1;
	m_iaButtonClickStamp[3] = -1;

	//try and acquire joypads
	for(int i = 0; i < JOYPADS_SUPPORTED; i++)
	{
		if( SUCCEEDED(m_pDirectInput->CreateDevice(GUID_Joystick, &m_pJoyPads[i], NULL)) )
		{
			m_pJoyPads[i]->SetDataFormat(&c_dfDIJoystick);
			m_pJoyPads[i]->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			m_pJoyPads[i]->Acquire();
			m_pJoyPads[i]->GetDeviceState(sizeof(DIJOYSTATE), &m_JoyStates[i]);
		}
		else
			m_pJoyPads[i] = 0;
	}
	

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
	if( SUCCEEDED(m_pKeyboard->GetDeviceState(sizeof(newKeyState), &newKeyState)) )
	{
		//check new input against old
		for(u32 i = 0; i < 256; i++ )
		{
			if( m_aKeyState[i] != newKeyState[i] )
			{
				if( m_aKeyState[i] != 0 )
					InputEvent(eIET_ButtonUp, i);

				if( newKeyState[i] != 0 )
					InputEvent(eIET_ButtonDown, i);
			}

			m_aKeyState[i] = newKeyState[i];
		}		
	}

	m_pKeyboard->Acquire();

	//Update mouse timers
	for(int i = 0; i < MICE_BUTTONS_SUPPORTED; i++)
	{
		if( m_iaButtonPressStamp[i] > -1)	
		{
			m_iaButtonPressStamp[i]++;		

			if( m_iaButtonPressStamp[i] > CLICK_DELTA )
				m_iaButtonPressStamp[i] = -1;
		}
		

		if( m_iaButtonClickStamp[i] > -1 )
		{
			m_iaButtonClickStamp[i]++;

			if( m_iaButtonClickStamp[i] > CLICK_DELTA )
				m_iaButtonClickStamp[i] = -1;
		}
	}

	//Check for new state
	DIMOUSESTATE newMouseState;
	if( SUCCEEDED(m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &newMouseState)) )
	{
		POINT mousePos;	
		GetCursorPos(&mousePos);		
		ScreenToClient(m_hWnd, &mousePos);	

		if( mousePos.x != m_vMousePos.GetX() || mousePos.y != m_vMousePos.GetY())
		{
			m_vMouseDelta.SetX(mousePos.x - m_vMousePos.GetX());
			m_vMouseDelta.SetY(mousePos.y - m_vMousePos.GetY());

			if( newMouseState.rgbButtons[0] != 0 )
				InputEvent(eIET_MouseDrag, 0);
			else
				InputEvent(eIET_MouseMove, 0);
		}
		else
			m_vMouseDelta = kpuv_Zero;

		m_vMousePos.SetX(mousePos.x);		
		m_vMousePos.SetY(mousePos.y);

		for(u32 i = 0; i < MICE_BUTTONS_SUPPORTED; i++ )
		{
			//check new input against old
			if( newMouseState.rgbButtons[i] != m_MouseState.rgbButtons[i] )
			{
				if( m_MouseState.rgbButtons[i] != 0 )
				{
					//mouse was released

					//See if double click was made
					if( m_iaButtonClickStamp[i] <= CLICK_DELTA && m_iaButtonClickStamp[i] > -1)
					{
						InputEvent(eIET_ButtonDoubleClick, KPIM_BUTTON_0 + 1);
						m_iaButtonClickStamp[i] = -1;
						m_iaButtonPressStamp[i] = -1;
						continue;
					}
					
					//See if click was made
					if( m_iaButtonPressStamp[i] <= CLICK_DELTA && m_iaButtonPressStamp[i] > -1)
					{
						InputEvent(eIET_ButtonClick, KPIM_BUTTON_0 + i);
						m_iaButtonClickStamp[i] = 0;
						m_iaButtonPressStamp[i] = -1;
						continue;
					}

					InputEvent(eIET_ButtonUp, KPIM_BUTTON_0 + i );
				}
				
				if( newMouseState.rgbButtons[i] != 0 )
				{
					InputEvent(eIET_ButtonDown, KPIM_BUTTON_0 + i);
					m_iaButtonPressStamp[i] = 0;
				}
			}
		}	

		m_MouseState = newMouseState;
	}

	m_pMouse->Acquire();

	// Update joypad states
	for(int i = 0; i < JOYPADS_SUPPORTED; i++)
	{
		//make sure pad exisit
		if( m_pJoyPads[i] )
		{
			//check new state	
			DIJOYSTATE newState;
			if( SUCCEEDED(m_pJoyPads[i]->GetDeviceState(sizeof(DIJOYSTATE), &newState)) )
			{				
				for(u32 u = 0; u < JOYPAD_BUTTONS_SUPPORTED; u++ )
				{
					if( newState.rgbButtons[u] != m_JoyStates[i].rgbButtons[u] )
					{
						if( m_JoyStates[i].rgbButtons[u] != 0 )
							InputEvent(eIET_ButtonUp, KPIJ_BUTTON_0 + u);

						if( newState.rgbButtons[u] != 0 )
							InputEvent(eIET_ButtonDown, KPIJ_BUTTON_0 + u);
					}			
				}	

				m_JoyStates[i] = newState;
			}

			m_pJoyPads[i]->Acquire();
			
		}
	}

	return true;
}