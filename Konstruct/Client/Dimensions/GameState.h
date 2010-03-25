#pragma once

class GameState
{
public:
	GameState(void)					{};
	virtual ~GameState(void)		{};

	virtual void MouseUpdate(int X, int Y) = 0;

	virtual void Update(float fGameTime) = 0;
	virtual void Draw() = 0;
};
