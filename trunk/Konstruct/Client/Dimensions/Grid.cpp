#include "StdAfx.h"
#include "Grid.h"
#include "Actor.h"
#include "Level.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Graphics/kpgVertexBuffer.h"
#include "Common/Graphics/kpgGeometryInstance.h"
#include "Common/Graphics/kpgGeometry.h"
#include "Common/Utility/kpuQuadTree.h"
#include "Common/Utility/kpuBoundingCapsule.h"



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

bool Grid::BuildPath(int iStartTile, int& iEndTile, int* outTiles,  int maxOutSize, Actor* pActor, int outTilesSize, int iLastDirection)
{
	// NOTE: This function will build a path backwards on itself if it cant go forward
	//			Will need to fix this at some point probably.

	//Make sure the end tile is a walkable choice to even move to
	/*if(!m_pTiles[iEndTile].m_bWalkable)
		return false;*/

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
			if( !m_pTiles[iTile].m_Actor )
			{
				bool bValid = true;

				//check if the tile is already in the list
				for(int i = 0; i < outTilesSize; i++)
				{
					if(outTiles[i] == iTile)
					{
						bValid = false;
						break;
					}
				}				

				if( bValid )
				{
					//Check if there is anything that will collide in the path
					kpuVector v2;
					GetTileLocation(iTile, v2);
					kpuBoundingCapsule capsule(vStartTile, v2, TILE_SIZE * 0.5f);

					if( !g_pGameState->GetLevel()->GetQuadTree()->CheckCollision(capsule, pActor) )
					{
						// This tile doesnt have an actor on it, its valid for now
						fBestDot = fDot;
						iBestDirection = i;
						iBestTile = iTile;
					}
				}
				

			}
			/*else if( iTile == iEndTile )
			{
				//If the next tile is the target tile but it is occupied return the path up to that tile
				iEndTile = iStartTile;
				outTiles[0] = iEndTile;
				if( outTilesSize >= 2 )
					outTiles[1] = -1;
				return true;
			}	*/		
		}
	}

	if( iBestDirection < 0 )
		return false;		// Failed to find anywhere to move

	if( iBestTile == iEndTile )
	{
		// Reached the destination, just add this to the list and be done
		outTiles[outTilesSize] = iBestTile;
		if( maxOutSize - outTilesSize >= 2 )
			outTiles[outTilesSize + 1] = -1;
		return true;
	}

	

	if( iLastDirection != iBestDirection )
	{
		// Changed directions, add a waypoint
		outTiles[outTilesSize] = iBestTile;
		//outTiles++;
		outTilesSize++;
		if( outTilesSize >= maxOutSize )
			return true;			// Done building path, no more space
	}

	// Find another tile along the path
	return BuildPath(iBestTile, iEndTile, outTiles, maxOutSize, pActor, outTilesSize, iBestDirection);
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

bool Grid::TileWalkable(int iTile)
{
	if( iTile < 0 || iTile >= m_iHeight * m_iWidth )
		return false;

	if( m_pTiles[iTile].m_Actor )
		return false;

	return true;
}

int Grid::Distance(Actor* pActor1, Actor* pActor2)
{
	int iTile = GetTileAtLocation(pActor1->GetLocation());
	kpuVector v1;
	GetTileLocation(iTile, v1);

	iTile = GetTileAtLocation(pActor2->GetLocation());
	kpuVector v2;
	GetTileLocation(iTile, v2);

	return  (int)sqrt(kpuVector::DistanceSquared(v1, v2));
}


