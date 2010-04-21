#include "StdAfx.h"
#include "Grid.h"
#include "Actor.h"
#include "TerrainModule.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Graphics/kpgVertexBuffer.h"
#include "Common/Graphics/kpgGeometryInstance.h"
#include "Common/Graphics/kpgGeometry.h"

Grid::Grid(int iWidth, int iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	// Allocate the tiles
	m_pTiles = (sGridTile*)calloc(sizeof(sGridTile), iWidth * iHeight);

	int iHalfWidth = iWidth / 2;
	int iHalfHeight = iHeight / 2;
	m_vCenter.Set((float)iHalfWidth, 0, (float)iHalfHeight, 1.0f);	
}

Grid::~Grid(void)
{
	if(m_pTiles)
	{
		free(m_pTiles);
		m_pTiles = 0;
	}
}

int Grid::GetTileAtLocation(const kpuVector& vLocation)
{
	// Convert to local space
	kpuVector vLocal = m_vCenter + vLocation;

	int iLocX = (int)vLocal.GetX();
	int iLocY = (int)vLocal.GetZ();
	return (iLocY * m_iWidth) + iLocX;
}

void Grid::GetTileLocation(int iTileIndex, kpuVector& vOutLocation)
{
	int iY = iTileIndex / m_iWidth;
	int iX = iTileIndex % m_iWidth;

	vOutLocation.Set((float)iX + 0.5f, 0, (float)iY + 0.5f, 0.0f);
	vOutLocation -= m_vCenter;
}

bool Grid::BuildPath(int iStartTile, int& iEndTile, int* outTiles, int outTilesSize, int iLastDirection)
{
	// NOTE: This function will build a path backwards on itself if it cant go forward
	//			Will need to fix this at some point probably.

	//Make sure the end tile is a walkable choice to even move to
	if(!m_pTiles[iEndTile].m_bWalkable)
		return false;

	kpuVector vStartTile, vEndTile;
	GetTileLocation(iStartTile, vStartTile);
	GetTileLocation(iEndTile, vEndTile);

	kpuVector vStartToEnd = vEndTile - vStartTile;
	vStartToEnd.Normalize();

	// Find the best next tile
	int iBestDirection = -1;
	float fBestDot = -2;
	int iBestTile = -1;
	for( int i = 0; i < NUM_DIRECTIONS; i++ )
	{
		float fDot = vStartToEnd.Dot(s_vDirections[i]);
		if( fDot > fBestDot )
		{
			int iTile = GetTileAtLocation(vStartTile + s_vDirections[i]);
			if( !m_pTiles[iTile].m_Actor && m_pTiles[iTile].m_bWalkable)
			{
				// This tile doesnt have an actor on it, its valid for now
				fBestDot = fDot;
				iBestDirection = i;
				iBestTile = iTile;
			}
			else if( iTile == iEndTile )
			{
				//If the next tile is the target tile but it is occupied return the path up to that tile
				iEndTile = iStartTile;
				outTiles[0] = iEndTile;
				if( outTilesSize >= 2 )
					outTiles[1] = -1;
				return true;
			}
			

			
		}
	}
	if( iBestDirection < 0 )
		return false;		// Failed to find anywhere to move

	if( iBestTile == iEndTile )
	{
		// Reached the destination, just add this to the list and be done
		outTiles[0] = iBestTile;
		if( outTilesSize >= 2 )
			outTiles[1] = -1;
		return true;
	}

	if( iLastDirection != iBestDirection )
	{
		// Changed directions, add a waypoint
		outTiles[0] = iBestTile;
		outTiles++;
		outTilesSize--;
		if( outTilesSize <= 0 )
			return true;			// Done building path, no more space
	}

	// Find another tile along the path
	return BuildPath(iBestTile, iEndTile, outTiles, outTilesSize, iBestDirection);
}

bool Grid::AddActor(Actor* pActor)
{
	int iTile = GetTileAtLocation(pActor->GetLocation());

	if(m_pTiles[iTile].m_Actor)
		return false;

	m_pTiles[iTile].m_Actor = pActor;
	return true;
}

bool Grid::RemoveActor(Actor* pActor)
{
	int iTile = GetTileAtLocation(pActor->GetLocation());

	if(!m_pTiles[iTile].m_Actor)
		return false;

	m_pTiles[iTile].m_Actor = 0;
	return true;
}

int Grid::AddTerrain(TerrainModule* pTerrain, int iTile)
{
	//Get Dimensions
	kpuVector vDimensions = pTerrain->GetDimensions();

	//Move terrain to the given tile
	kpuVector vLocation;

	//Get and set the new location of the model
	GetTileLocation(iTile, vLocation);
	vLocation -= kpuVector(0.5f, 0, 0.5f, 0);
	vLocation += vDimensions * 0.5f;
	pTerrain->GetModel()->SetPosition(vLocation);

	//Make sure the object fits in the map
	if( iTile % m_iWidth + vDimensions.GetX() > m_iWidth )
		return iTile;
	if( iTile + vDimensions.GetZ() > GetNumOfTiles() )
		return iTile;

	//Array to hold the tiles this terrain takes up, in case we have to undo what we did
	kpuFixedArray<sGridTile*> tilesUsed;
	tilesUsed.SetSize(vDimensions.GetX() * vDimensions.GetZ());

	int iRet = -1;

	//try and fill in tiles 
	for(int i = 0; i < vDimensions.GetZ(); i++)
	{
		for(int j = 0; j < vDimensions.GetX(); j++)
		{
			//Get next tile
			//int iNextTile = GetTileAtLocation( kpuVector(j, 0, i, 0) + vLocation );

			int iNextTile = iTile + ( i * m_iWidth ) + j;

			if(m_pTiles[iNextTile].m_bTerrain)
			{
				//Already covered so can't go here
				iRet = iTile;
				break;
			}


			m_pTiles[iNextTile].m_bTerrain = true;
			m_pTiles[iNextTile].m_bWalkable = true;
			tilesUsed.Add(&m_pTiles[iNextTile]);

		}

		

		if(iRet == iTile)
			break;
	}

	//Fill in any tiles with walls as unwalkable
	kpuArrayList<kpuVector>* paWalls = pTerrain->GetWalls();

	for(int i = 0; i < paWalls->Count(); i++)
	{
		kpuVector vWall = (*paWalls)[i];

		//Unnormalize the sides of the terrain the doors are on
		vWall.SetX( vWall.GetX() * ( vDimensions.GetX() - 1 ) );
		vWall.SetY( vWall.GetY() * ( vDimensions.GetZ() - 1 ) );
		
		int iNextTile = iTile;

		iNextTile += vWall.GetX();
		iNextTile += vWall.GetY() * m_iWidth;			

		//Add offset from corner
		iNextTile += vWall.GetZ();

		for(int j = 0; j < vWall.GetW(); j++)
		{
			m_pTiles[iNextTile].m_bWalkable = false;

			int iIncrement = 1;

			//if we are at the max Z then the move will be negaive from here
			if( vWall.GetY() == vDimensions.GetZ() - 1 )
				iIncrement *= -1;

			if( ( vWall.GetX() == vDimensions.GetX() - 1 && vWall.GetY() == vDimensions.GetZ() - 1 ) || ( vWall.GetX() == 0.0f && vWall.GetY() == 0.0f ))
					iNextTile += iIncrement;
			else
				iNextTile += m_iWidth * iIncrement;

			
		}


	}

	//Now fill in any doors
	kpuArrayList<kpuVector>* paDoors = pTerrain->GetDoors();

	for(int i = 0; i < paDoors->Count(); i++)
	{
		kpuVector vDoor = (*paDoors)[i];		

		//Unnormalize the sides of the terrain the doors are on
		vDoor.SetX( vDoor.GetX() * ( vDimensions.GetX() - 1 ) );
		vDoor.SetZ( vDoor.GetZ() * ( vDimensions.GetZ() - 1 ) );
		
		int iDoorTile = iTile;

		iDoorTile += vDoor.GetX();
		iDoorTile += vDoor.GetZ() * m_iWidth;

		int iSign = 1;

		//if we are at the max Z then the move will be negaive from here
		if( vDoor.GetZ() == vDimensions.GetZ() - 1 )
			iSign = -1;	

		int iTestTile = iDoorTile;

		if( ( vDoor.GetX() == vDimensions.GetX() - 1 && vDoor.GetZ() == vDimensions.GetZ() - 1 ) || ( vDoor.GetX() == 0.0f && vDoor.GetZ() == 0.0f ))
		{				
			iDoorTile += vDoor.GetW() * iSign;
			iTestTile = iDoorTile + m_iWidth * -iSign;
		}
		else
		{
			iDoorTile += m_iWidth * vDoor.GetW() * iSign;
			iTestTile = iDoorTile + -iSign;
		}

		//Make sure tile doesn't lead off the grid and make sure if it isn't walkable it isn't covered by another peice of terrain
		if( ( iDoorTile % m_iWidth - 1 == 0 || iDoorTile % m_iWidth == 0 || iTestTile > GetNumOfTiles() || iTestTile < 0 )
					|| ( !m_pTiles[iTestTile].m_bWalkable &&  m_pTiles[iTestTile].m_bTerrain ) )
		{
			iRet = iTile;
			break;
		}

		m_pTiles[iDoorTile].m_bWalkable = true;
	}

	//tile cannot go here, undo what we changed
	if(iRet == iTile)
	{
		for(int i = 0 ; i < tilesUsed.GetNumElementsUsed(); i++)
		{
			tilesUsed[i]->m_bTerrain = false;
			tilesUsed[i]->m_bWalkable = false;
		}

		return iRet;
	}
	
	//Get the next tile to start from
	iTile += vDimensions.GetX();
		
	//If next tile is off the grid move down a level
	if( iTile % m_iWidth == 0 )
	{
		iTile = 0;

		while(iTile < GetNumOfTiles() && m_pTiles[iTile].m_bTerrain )
			iTile += m_iWidth;

	}
	
	return iTile;
}

bool Grid::IsWalkable(int iTile)
{
	if( iTile < 0 || iTile > GetNumOfTiles() )
		return false;

	return m_pTiles[iTile].m_bWalkable;
}