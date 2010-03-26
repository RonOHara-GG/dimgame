#include "StdAfx.h"
#include "Tile.h"

Tile::Tile(int X, int Z, int iGridWidth, int iGridHeight, int iTile)
{
	m_vCenter = kpuVector(X + TILE_SIZE / 2.0f, 0.0f, Z + TILE_SIZE / 2.0f, 0.0f);
	m_bWalkable = true;

	int iColumn = iTile % m_iWidth;

	m_Cords = kpuGridCord(iColumn, (iTile - iColumn) / m_iWidth);

	m_iTileNum = iTile;
}

Tile::~Tile(void)
{
}




