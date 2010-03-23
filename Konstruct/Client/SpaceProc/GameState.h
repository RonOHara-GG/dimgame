#pragma once

class GameState
{
public:
	GameState(void)					{};
	virtual ~GameState(void)		{};

	virtual void Update() = 0;
	virtual void Draw() = 0;
};
