#pragma once

class kpgRenderer;

class kpgUIManager
{
public:
	kpgUIManager(void);
	~kpgUIManager(void);

	void Update();
	void Draw(kpgRenderer* pRenderer);
};
