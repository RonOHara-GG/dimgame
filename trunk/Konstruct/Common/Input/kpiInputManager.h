#pragma once

#define DIRECTINPUT_VERSION         0x0800

#include <d3d9.h>
#include <dinput.h>
#include "Common/Utility/kpuVector.h"

enum eInputEventType
{
	eIET_MouseMove,
	eIET_MouseDrag,
	eIET_ButtonDown,
	eIET_ButtonUp,
	eIET_ButtonRepeat,
	eIET_ButtonClick,
	eIET_ButtonDoubleClick,
	eIET_KeyPress
};

#define MICE_BUTTONS_SUPPORTED 4
#define JOYPADS_SUPPORTED 4
#define JOYPAD_BUTTONS_SUPPORTED 32

class kpiInputManager
{
public:

	kpiInputManager( HWND window );
	~kpiInputManager(void);

	bool Update(float fDeltaTime);

	kpPoint		GetMouseLoc() { return m_ptMousePos; }
	kpPoint		GetMouseDelta() { return m_ptMouseDelta; }

protected:
	HWND					m_hWnd;
	IDirectInput8*			m_pDirectInput;
	ul32					m_ulPressStamp;

	//keyboard input
	IDirectInputDevice8*	m_pKeyboard;	
	BYTE					m_aKeyState[256];
	

	//Mouse input
	IDirectInputDevice8*	m_pMouse;	
	DIMOUSESTATE			m_MouseState;
	float					m_faButtonPressStamp[MICE_BUTTONS_SUPPORTED];
	bool					m_bClicked;
	kpPoint					m_ptMousePos;
	kpPoint					m_ptMouseDelta;

	//Joypads
	IDirectInputDevice8*	m_pJoyPads[JOYPADS_SUPPORTED];
	DIJOYSTATE				m_JoyStates[JOYPADS_SUPPORTED];

};

extern void	InputEvent(eInputEventType type, u32 button);
extern kpiInputManager* g_pInputManager;

// Timing for clicks in frames
#define CLICK_DELTA 0.5f

#define KPIK_ESCAPE          0x001
#define KPIK_1               0x002
#define KPIK_2               0x003
#define KPIK_3               0x004
#define KPIK_4               0x005
#define KPIK_5               0x006
#define KPIK_6               0x007
#define KPIK_7               0x008
#define KPIK_8               0x009
#define KPIK_9               0x00A
#define KPIK_0               0x00B
#define KPIK_MINUS           0x00C    /* - on main keyboard */
#define KPIK_EQUALS          0x00D
#define KPIK_BACK            0x00E    /* backspace */
#define KPIK_TAB             0x00F
#define KPIK_Q               0x010
#define KPIK_W               0x011
#define KPIK_E               0x012
#define KPIK_R               0x013
#define KPIK_T               0x014
#define KPIK_Y               0x015
#define KPIK_U               0x016
#define KPIK_I               0x017
#define KPIK_O               0x018
#define KPIK_P               0x019
#define KPIK_LBRACKET        0x01A
#define KPIK_RBRACKET        0x01B
#define KPIK_RETURN          0x01C    /* Enter on main keyboard */
#define KPIK_LCONTROL        0x01D
#define KPIK_A               0x01E
#define KPIK_S               0x01F
#define KPIK_D               0x020
#define KPIK_F               0x021
#define KPIK_G               0x022
#define KPIK_H               0x023
#define KPIK_J               0x024
#define KPIK_K               0x025
#define KPIK_L               0x026
#define KPIK_SEMICOLON       0x027
#define KPIK_APOSTROPHE      0x028
#define KPIK_GRAVE           0x029    /* accent grave */
#define KPIK_LSHIFT          0x02A
#define KPIK_BACKSLASH       0x02B
#define KPIK_Z               0x02C
#define KPIK_X               0x02D
#define KPIK_C               0x02E
#define KPIK_V               0x02F
#define KPIK_B               0x030
#define KPIK_N               0x031
#define KPIK_M               0x032
#define KPIK_COMMA           0x033
#define KPIK_PERIOD          0x034    /* . on main keyboard */
#define KPIK_SLASH           0x035    /* / on main keyboard */
#define KPIK_RSHIFT          0x036
#define KPIK_MULTIPLY        0x037    /* * on numeric keypad */
#define KPIK_LMENU           0x038    /* left Alt */
#define KPIK_SPACE           0x039
#define KPIK_CAPITAL         0x03A
#define KPIK_F1              0x03B
#define KPIK_F2              0x03C
#define KPIK_F3              0x03D
#define KPIK_F4              0x03E
#define KPIK_F5              0x03F
#define KPIK_F6              0x040
#define KPIK_F7              0x041
#define KPIK_F8              0x042
#define KPIK_F9              0x043
#define KPIK_F10             0x044
#define KPIK_NUMLOCK         0x045
#define KPIK_SCROLL          0x046    /* Scroll Lock */
#define KPIK_NUMPAD7         0x047
#define KPIK_NUMPAD8         0x048
#define KPIK_NUMPAD9         0x049
#define KPIK_SUBTRACT        0x04A    /* - on numeric keypad */
#define KPIK_NUMPAD4         0x04B
#define KPIK_NUMPAD5         0x04C
#define KPIK_NUMPAD6         0x04D
#define KPIK_ADD             0x04E    /* + on numeric keypad */
#define KPIK_NUMPAD1         0x04F
#define KPIK_NUMPAD2         0x050
#define KPIK_NUMPAD3         0x051
#define KPIK_NUMPAD0         0x052
#define KPIK_DECIMAL         0x053    /* . on numeric keypad */
#define KPIK_OEM_102         0x056    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
#define KPIK_F11             0x057
#define KPIK_F12             0x058
#define KPIK_F13             0x064    /*                     (NEC PC98) */
#define KPIK_F14             0x065    /*                     (NEC PC98) */
#define KPIK_F15             0x066    /*                     (NEC PC98) */
#define KPIK_KANA            0x070    /* (Japanese keyboard)            */
#define KPIK_ABNT_C1         0x073    /* /? on Brazilian keyboard */
#define KPIK_CONVERT         0x079    /* (Japanese keyboard)            */
#define KPIK_NOCONVERT       0x07B    /* (Japanese keyboard)            */
#define KPIK_YEN             0x07D    /* (Japanese keyboard)            */
#define KPIK_ABNT_C2         0x07E    /* Numpad . on Brazilian keyboard */
#define KPIK_NUMPADEQUALS    0x08D    /* = on numeric keypad (NEC PC98) */
#define KPIK_PREVTRACK       0x090    /* Previous Track (KPIK_CIRCUMFLEX on Japanese keyboard) */
#define KPIK_AT              0x091    /*                     (NEC PC98) */
#define KPIK_COLON           0x092    /*                     (NEC PC98) */
#define KPIK_UNDERLINE       0x093    /*                     (NEC PC98) */
#define KPIK_KANJI           0x094    /* (Japanese keyboard)            */
#define KPIK_STOP            0x095    /*                     (NEC PC98) */
#define KPIK_AX              0x096    /*                     (Japan AX) */
#define KPIK_UNLABELED       0x097    /*                        (J3100) */
#define KPIK_NEXTTRACK       0x099    /* Next Track */
#define KPIK_NUMPADENTER     0x09C    /* Enter on numeric keypad */
#define KPIK_RCONTROL        0x09D
#define KPIK_MUTE            0x0A0    /* Mute */
#define KPIK_CALCULATOR      0x0A1    /* Calculator */
#define KPIK_PLAYPAUSE       0x0A2    /* Play / Pause */
#define KPIK_MEDIASTOP       0x0A4    /* Media Stop */
#define KPIK_VOLUMEDOWN      0x0AE    /* Volume - */
#define KPIK_VOLUMEUP        0x0B0    /* Volume + */
#define KPIK_WEBHOME         0x0B2    /* Web home */
#define KPIK_NUMPADCOMMA     0x0B3    /* , on numeric keypad (NEC PC98) */
#define KPIK_DIVIDE          0x0B5    /* / on numeric keypad */
#define KPIK_SYSRQ           0x0B7
#define KPIK_RMENU           0x0B8    /* right Alt */
#define KPIK_PAUSE           0x0C5    /* Pause */
#define KPIK_HOME            0x0C7    /* Home on arrow keypad */
#define KPIK_UP              0x0C8    /* UpArrow on arrow keypad */
#define KPIK_PRIOR           0x0C9    /* PgUp on arrow keypad */
#define KPIK_LEFT            0x0CB    /* LeftArrow on arrow keypad */
#define KPIK_RIGHT           0x0CD    /* RightArrow on arrow keypad */
#define KPIK_END             0x0CF    /* End on arrow keypad */
#define KPIK_DOWN            0x0D0    /* DownArrow on arrow keypad */
#define KPIK_NEXT            0x0D1    /* PgDn on arrow keypad */
#define KPIK_INSERT          0x0D2    /* Insert on arrow keypad */
#define KPIK_DELETE          0x0D3    /* Delete on arrow keypad */
#define KPIK_LWIN            0x0DB    /* Left Windows key */
#define KPIK_RWIN            0x0DC    /* Right Windows key */
#define KPIK_APPS            0x0DD    /* AppMenu key */
#define KPIK_POWER           0x0DE    /* System Power */
#define KPIK_SLEEP           0x0DF    /* System Sleep */
#define KPIK_WAKE            0x0E3    /* System Wake */
#define KPIK_WEBSEARCH       0x0E5    /* Web Search */
#define KPIK_WEBFAVORITES    0x0E6    /* Web Favorites */
#define KPIK_WEBREFRESH      0x0E7    /* Web Refresh */
#define KPIK_WEBSTOP         0x0E8    /* Web Stop */
#define KPIK_WEBFORWARD      0x0E9    /* Web Forward */
#define KPIK_WEBBACK         0x0EA    /* Web Back */
#define KPIK_MYCOMPUTER      0x0EB    /* My Computer */
#define KPIK_MAIL            0x0EC    /* Mail */
#define KPIK_MEDIASELECT     0x0ED    /* Media Select */

//Mouse buttons
#define KPIM_BUTTON_0		 0x0EE  /* Left Mouse */
#define KPIM_BUTTON_1		 0x0EF  /* Right Mouse */
#define KPIM_BUTTON_2		 0x0F0  /* Extra Mouse Button */
#define KPIM_BUTTON_3		 0x0F1  /* Extra Mouse Button */

//Joypad buttons
#define KPIJ_BUTTON_0		 0x0F2
#define KPIJ_BUTTON_1		 0x0F3
#define KPIJ_BUTTON_2		 0x0F4
#define KPIJ_BUTTON_3		 0x0F5
#define KPIJ_BUTTON_4		 0x0F6
#define KPIJ_BUTTON_5		 0x0F7
#define KPIJ_BUTTON_6		 0x0F8
#define KPIJ_BUTTON_7		 0x0F9
#define KPIJ_BUTTON_8		 0x0FA
#define KPIJ_BUTTON_9		 0x0FB
#define KPIJ_BUTTON_10		 0x0FC
#define KPIJ_BUTTON_11		 0x0FD
#define KPIJ_BUTTON_12		 0x0FE
#define KPIJ_BUTTON_13		 0x0FF
#define KPIJ_BUTTON_14		 0x100
#define KPIJ_BUTTON_15		 0x101
#define KPIJ_BUTTON_16		 0x102
#define KPIJ_BUTTON_17		 0x103
#define KPIJ_BUTTON_18		 0x104
#define KPIJ_BUTTON_19		 0x105
#define KPIJ_BUTTON_20		 0x106
#define KPIJ_BUTTON_21		 0x107
#define KPIJ_BUTTON_22		 0x108
#define KPIJ_BUTTON_23		 0x109
#define KPIJ_BUTTON_24		 0x10A
#define KPIJ_BUTTON_25		 0x10B
#define KPIJ_BUTTON_26		 0x10C
#define KPIJ_BUTTON_27		 0x10D
#define KPIJ_BUTTON_28		 0x10E
#define KPIJ_BUTTON_29		 0x10F
#define KPIJ_BUTTON_30		 0x110
#define KPIJ_BUTTON_31		 0x111


