#pragma once

enum eInputEventType
{
	eIET_ButtonDown,
	eIET_ButtonUp,
	eIET_ButtonRepeat,
};

class kpiInputManager
{
public:

	kpiInputManager(void);
	~kpiInputManager(void);

	bool Update();
};

extern void	InputEvent(eInputEventType type, u32 button);
