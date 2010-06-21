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
	BYTE					m_aKeyState[256];
	ul32					m_aulKeyPressStamp[256];

	//Mouse input
	IDirectInputDevice8*	m_pMouse;	
	DIMOUSESTATE			m_MouseState;
	ul32					m_ulButtonPressStamp[3];
	kpuVector				m_vMousePos;

};

extern void	InputEvent(eInputEventType type, u32 button);

#define KPIK_ESCAPE          0x01
#define KPIK_1               0x02
#define KPIK_2               0x03
#define KPIK_3               0x04
#define KPIK_4               0x05
#define KPIK_5               0x06
#define KPIK_6               0x07
#define KPIK_7               0x08
#define KPIK_8               0x09
#define KPIK_9               0x0A
#define KPIK_0               0x0B
#define KPIK_MINUS           0x0C    /* - on main keyboard */
#define KPIK_EQUALS          0x0D
#define KPIK_BACK            0x0E    /* backspace */
#define KPIK_TAB             0x0F
#define KPIK_Q               0x10
#define KPIK_W               0x11
#define KPIK_E               0x12
#define KPIK_R               0x13
#define KPIK_T               0x14
#define KPIK_Y               0x15
#define KPIK_U               0x16
#define KPIK_I               0x17
#define KPIK_O               0x18
#define KPIK_P               0x19
#define KPIK_LBRACKET        0x1A
#define KPIK_RBRACKET        0x1B
#define KPIK_RETURN          0x1C    /* Enter on main keyboard */
#define KPIK_LCONTROL        0x1D
#define KPIK_A               0x1E
#define KPIK_S               0x1F
#define KPIK_D               0x20
#define KPIK_F               0x21
#define KPIK_G               0x22
#define KPIK_H               0x23
#define KPIK_J               0x24
#define KPIK_K               0x25
#define KPIK_L               0x26
#define KPIK_SEMICOLON       0x27
#define KPIK_APOSTROPHE      0x28
#define KPIK_GRAVE           0x29    /* accent grave */
#define KPIK_LSHIFT          0x2A
#define KPIK_BACKSLASH       0x2B
#define KPIK_Z               0x2C
#define KPIK_X               0x2D
#define KPIK_C               0x2E
#define KPIK_V               0x2F
#define KPIK_B               0x30
#define KPIK_N               0x31
#define KPIK_M               0x32
#define KPIK_COMMA           0x33
#define KPIK_PERIOD          0x34    /* . on main keyboard */
#define KPIK_SLASH           0x35    /* / on main keyboard */
#define KPIK_RSHIFT          0x36
#define KPIK_MULTIPLY        0x37    /* * on numeric keypad */
#define KPIK_LMENU           0x38    /* left Alt */
#define KPIK_SPACE           0x39
#define KPIK_CAPITAL         0x3A
#define KPIK_F1              0x3B
#define KPIK_F2              0x3C
#define KPIK_F3              0x3D
#define KPIK_F4              0x3E
#define KPIK_F5              0x3F
#define KPIK_F6              0x40
#define KPIK_F7              0x41
#define KPIK_F8              0x42
#define KPIK_F9              0x43
#define KPIK_F10             0x44
#define KPIK_NUMLOCK         0x45
#define KPIK_SCROLL          0x46    /* Scroll Lock */
#define KPIK_NUMPAD7         0x47
#define KPIK_NUMPAD8         0x48
#define KPIK_NUMPAD9         0x49
#define KPIK_SUBTRACT        0x4A    /* - on numeric keypad */
#define KPIK_NUMPAD4         0x4B
#define KPIK_NUMPAD5         0x4C
#define KPIK_NUMPAD6         0x4D
#define KPIK_ADD             0x4E    /* + on numeric keypad */
#define KPIK_NUMPAD1         0x4F
#define KPIK_NUMPAD2         0x50
#define KPIK_NUMPAD3         0x51
#define KPIK_NUMPAD0         0x52
#define KPIK_DECIMAL         0x53    /* . on numeric keypad */
#define KPIK_OEM_102         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
#define KPIK_F11             0x57
#define KPIK_F12             0x58
#define KPIK_F13             0x64    /*                     (NEC PC98) */
#define KPIK_F14             0x65    /*                     (NEC PC98) */
#define KPIK_F15             0x66    /*                     (NEC PC98) */
#define KPIK_KANA            0x70    /* (Japanese keyboard)            */
#define KPIK_ABNT_C1         0x73    /* /? on Brazilian keyboard */
#define KPIK_CONVERT         0x79    /* (Japanese keyboard)            */
#define KPIK_NOCONVERT       0x7B    /* (Japanese keyboard)            */
#define KPIK_YEN             0x7D    /* (Japanese keyboard)            */
#define KPIK_ABNT_C2         0x7E    /* Numpad . on Brazilian keyboard */
#define KPIK_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
#define KPIK_PREVTRACK       0x90    /* Previous Track (KPIK_CIRCUMFLEX on Japanese keyboard) */
#define KPIK_AT              0x91    /*                     (NEC PC98) */
#define KPIK_COLON           0x92    /*                     (NEC PC98) */
#define KPIK_UNDERLINE       0x93    /*                     (NEC PC98) */
#define KPIK_KANJI           0x94    /* (Japanese keyboard)            */
#define KPIK_STOP            0x95    /*                     (NEC PC98) */
#define KPIK_AX              0x96    /*                     (Japan AX) */
#define KPIK_UNLABELED       0x97    /*                        (J3100) */
#define KPIK_NEXTTRACK       0x99    /* Next Track */
#define KPIK_NUMPADENTER     0x9C    /* Enter on numeric keypad */
#define KPIK_RCONTROL        0x9D
#define KPIK_MUTE            0xA0    /* Mute */
#define KPIK_CALCULATOR      0xA1    /* Calculator */
#define KPIK_PLAYPAUSE       0xA2    /* Play / Pause */
#define KPIK_MEDIASTOP       0xA4    /* Media Stop */
#define KPIK_VOLUMEDOWN      0xAE    /* Volume - */
#define KPIK_VOLUMEUP        0xB0    /* Volume + */
#define KPIK_WEBHOME         0xB2    /* Web home */
#define KPIK_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
#define KPIK_DIVIDE          0xB5    /* / on numeric keypad */
#define KPIK_SYSRQ           0xB7
#define KPIK_RMENU           0xB8    /* right Alt */
#define KPIK_PAUSE           0xC5    /* Pause */
#define KPIK_HOME            0xC7    /* Home on arrow keypad */
#define KPIK_UP              0xC8    /* UpArrow on arrow keypad */
#define KPIK_PRIOR           0xC9    /* PgUp on arrow keypad */
#define KPIK_LEFT            0xCB    /* LeftArrow on arrow keypad */
#define KPIK_RIGHT           0xCD    /* RightArrow on arrow keypad */
#define KPIK_END             0xCF    /* End on arrow keypad */
#define KPIK_DOWN            0xD0    /* DownArrow on arrow keypad */
#define KPIK_NEXT            0xD1    /* PgDn on arrow keypad */
#define KPIK_INSERT          0xD2    /* Insert on arrow keypad */
#define KPIK_DELETE          0xD3    /* Delete on arrow keypad */
#define KPIK_LWIN            0xDB    /* Left Windows key */
#define KPIK_RWIN            0xDC    /* Right Windows key */
#define KPIK_APPS            0xDD    /* AppMenu key */
#define KPIK_POWER           0xDE    /* System Power */
#define KPIK_SLEEP           0xDF    /* System Sleep */
#define KPIK_WAKE            0xE3    /* System Wake */
#define KPIK_WEBSEARCH       0xE5    /* Web Search */
#define KPIK_WEBFAVORITES    0xE6    /* Web Favorites */
#define KPIK_WEBREFRESH      0xE7    /* Web Refresh */
#define KPIK_WEBSTOP         0xE8    /* Web Stop */
#define KPIK_WEBFORWARD      0xE9    /* Web Forward */
#define KPIK_WEBBACK         0xEA    /* Web Back */
#define KPIK_MYCOMPUTER      0xEB    /* My Computer */
#define KPIK_MAIL            0xEC    /* Mail */
#define KPIK_MEDIASELECT     0xED    /* Media Select */

