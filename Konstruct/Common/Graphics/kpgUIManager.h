#pragma once

class kpgRenderer;
enum eInputEventType;

class kpgUIManager
{
public:
	kpgUIManager(void);
	~kpgUIManager(void);

	void Update();
	void Draw(kpgRenderer* pRenderer);

	bool HandleInputEvent(eInputEventType type, u32 button);
};
