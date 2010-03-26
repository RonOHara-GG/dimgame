#pragma once

#include "Common/Utility/kpuVector.h"
#include "Common/Utility/kpuGridCord.h"

class GameObject;

/**
*	This is a 2D container on the XZ plane to hold map data
*	Brett Flassing
**/
class Tile
{
public:
	Tile(int X, int Z, int iGridWidth, int iGridHeight, int iTile);
	~Tile(void);

	int			GetTileNum()	{ return m_iTileNum; }
	kpuGridCord	GetCords()		{ return m_Cords; }
	kpuVector	GetCenter()		{ return m_vCenter; }
	bool		IsWalkable()		{ return m_bWalkable; }

private:
	kpuGridCord			m_Cords;
	kpuVector			m_vCenter;
	bool				m_bWalkable;
	int					m_iTileNum;
	int					m_iWidth;
	int					m_iLength;

	//Things the tile can contain
	GameObject*			m_pobjObject;

	//Loot**			m_pltLoot; Any of the loot contained in the space
};

//The size of one tile
#define TILE_SIZE 1
