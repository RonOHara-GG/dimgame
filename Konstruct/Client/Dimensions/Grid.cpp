#include "StdAfx.h"
#include "Grid.h"


Grid::Grid(float fWidth, float fLength)
{

	//Make sure width and heigth are even
	if(fWidth - (int)fWidth > 0)	
		m_iWidth = (int)fWidth + 1;	

	if(fLength - (int)fLength > 0)
		m_iLength = (int)fLength+ 1;	
	
	m_pTiles = (Tile**)malloc(sizeof(Tile*) * m_iWidth * m_iLength);

	int iTile = 0;

	for(int z = 0; z < m_iLength; z++)
	{
		for(int x = 0; x < m_iWidth; x++)
		{
			m_pTiles[iTile] = new Tile(x, z, m_iWidth, m_iLength, iTile);			
			iTile++;
		}
	}
	
	
}

Grid::~Grid(void)
{
	if(m_pTiles)
		delete m_pTiles;
}


kpuArrayList<Tile>* Grid::FindPath(Tile* target, Tile* current)
{
	kpuArrayList<Tile>* openList = new kpuArrayList<Tile>();
	kpuArrayList<Tile>* closedList = new kpuArrayList<Tile>();
	
	openList->Add(*current);

	while(!closedList->Contains(*target) && openList->Count() > 0 )
	{
		int iCurrent = -1;
		//Get the closest tile to the target on the open list and make that the current	
	
		int iSmDist = m_iWidth * m_iLength;
		for(int i = 0; i < openList->Count(); i++)
		{
			Tile* nextTile = &(*openList)[i];

			int iDist = nextTile->GetCords().DistanceSquared(target->GetCords());

			if(iDist < iSmDist)
			{
				iDist = iSmDist;
				iCurrent = nextTile->GetTileNum();
				current = nextTile;
			}

		}

		closedList->Add(*current);
		//Get all adjacent squares and and add them to open list if they are walkable

		//Start at top left tile and go clockwise

		int iNext = iCurrent - m_iWidth - 1;
		if(m_pTiles[iNext]->IsWalkable())
			if(!openList->Contains(*m_pTiles[iNext]))
				openList->Add(*m_pTiles[iNext]);

		iNext = iCurrent - m_iWidth;
		if(m_pTiles[iNext]->IsWalkable())
			if(!openList->Contains(*m_pTiles[iNext]))
				openList->Add(*m_pTiles[iNext]);

		iNext = iCurrent - m_iWidth + 1;
		if(m_pTiles[iNext]->IsWalkable())
			if(!openList->Contains(*m_pTiles[iNext]))
				openList->Add(*m_pTiles[iNext]);
		iNext = iCurrent + 1;
		if(m_pTiles[iNext]->IsWalkable())
			if(!openList->Contains(*m_pTiles[iNext]))
				openList->Add(*m_pTiles[iNext]);

		iNext = iCurrent + m_iWidth + 1;
		if(m_pTiles[iNext]->IsWalkable())
			if(!openList->Contains(*m_pTiles[iNext]))
				openList->Add(*m_pTiles[iNext]);

		iNext = iCurrent + m_iWidth;
		if(m_pTiles[iNext]->IsWalkable())
			if(!openList->Contains(*m_pTiles[iNext]))
				openList->Add(*m_pTiles[iNext]);

		iNext = iCurrent - m_iWidth + 1;
		if(m_pTiles[iNext]->IsWalkable())
			if(!openList->Contains(*m_pTiles[iNext]))
				openList->Add(*m_pTiles[iNext]);

		iNext = iCurrent - 1;
		if(m_pTiles[iNext]->IsWalkable())
			if(!openList->Contains(*m_pTiles[iNext]))
				openList->Add(*m_pTiles[iNext]);
	}

	return closedList;
}

Tile* Grid::GetTile(const kpuVector &vGroundClick)
{
	if(m_pTiles)
	{
		//Get the tile that is being clicked
		int iCol = (int)vGroundClick.GetX();
		int iRow = (int)vGroundClick.GetZ();

		
		return m_pTiles[iRow * m_iWidth + iCol];		
	}

	return NULL;

}



