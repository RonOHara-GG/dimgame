#pragma once

#include "Common/Utility/kpuArrayList.h"
#include "Common/Utility/kpuVector.h"
#include "Common/Utility/kpuLinkedList.h"
#include "Common/Utility/kpuGridCord.h"




class Actor;

/**
*	This is a 2D grid use to spatially partition the tiles in the level.
*	Each Grid is broken down into 8 grids until the size is 1 then they are broken down into the level tiles
*	Brett Flassing
**/
class Grid
{
public:
	Grid(float fWidth, float fLength);
	~Grid(void);

	void		AddObject(Actor* obj); //Adds an new object to the grid
	void		BuildGrid(); //Builds the grid

	Tile*					GetTile(const kpuVector& vGroundClick); //Gets the tile that was clicked by the mouse
	kpuArrayList<Tile>*		FindPath(Tile* target, Tile* current);
	

private:
	//Divides the grid down to 1 X 1 tiles then returns a lists of all the tiles
	void Divide(kpuLinkedList* tileList);
	
	//The width and height of the current gird
	int				m_iWidth;
	int				m_iLength;

	//The tiles of the grid
	Tile**			m_pTiles;
};


