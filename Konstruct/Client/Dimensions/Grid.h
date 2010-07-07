#pragma once

#include "Common/Utility/kpuArrayList.h"
#include "Common/Utility/kpuVector.h"
#include "Common/Utility/kpuLinkedList.h"
#include "Common/Utility/kpuGridCord.h"
#include "Common/Graphics/kpgModel.h"

class Actor;
class GameObject;
class TerrainModule;

#define NUM_DIRECTIONS	8

static const kpuVector s_vDirections[NUM_DIRECTIONS] =
{
	kpuVector(1.0f,			0.0f,	0.0f,			0.0f),	// Right
	kpuVector(0.70710678f,	0.0f,	-0.70710678f,	0.0f),	// Right/Down
	kpuVector(0.0f,			0.0f,	-1.0f,			0.0f),	// Down
	kpuVector(-0.70710678f,	0.0f,	-0.70710678f,	0.0f),	// Down/Left
	kpuVector(-1.0f,		0.0f,	0.0f,			0.0f),	// Left
	kpuVector(-0.70710678f,	0.0f,	0.70710678f,	0.0f),	// Left/Up
	kpuVector(0.0f,			0.0f,	1.0f,			0.0f),	// Up
	kpuVector(0.70710678f,	0.0f,	0.70710678f,	0.0f),	// Up/Right	

};

class Grid
{
public:
	struct sGridTile
	{
		GameObject*		m_Item;			// One item allowed per tile
		Actor*			m_Actor;			// One actor allowed per tile
		bool			m_bWalkable;
		bool			m_bTerrain;		//Wether or not this tile is being occupied by a piece of terrain already
	};


	Grid(int iWidth, int iHeight);
	~Grid(void);

	int			GetNumOfTiles() { return m_iWidth * m_iHeight; }
	int			GetTileAtLocation(const kpuVector& vLocation);
	int			Distance(Actor* pActor1, Actor* pActor2);
	int			Distance(int iTile1, int iTile2);
	int			DistanceSquared(Actor* pActor1, Actor* pActor2);
	int			DistanceSquared(int iTile1, int iTile2);
	bool		TileWalkable(int iTile);
	void		GetTileLocation(int iTileIndex, kpuVector& vOutLocation);
	bool		AddActor(Actor* pActor); //Add actor to the gird, if an actor is already on the tile return false else return ture
	bool		RemoveActor(Actor* pActor);//Remove actor from his current tile, if he is not found on the current tile return false
	

	bool		BuildPath(int iStartTile, int& iEndTile, int* outTiles, int maxOutSize, Actor* pActor, int outTilesSize = 0, int iLastDirection = -1);
	void		BuildStraightPath(int iStartTile, int& iEndTile, int* outTiles, Actor* pActor, kpuVector vDirection);
	int			BestMove(kpuVector vNorm, int iStartTile);
	int			NearestTile(kpuVector vNorm, int iTile, Actor* pActor);
	GameObject* GetLoot(int iTile) { return m_pTiles[iTile].m_Item; } //Get the loot at the target tile
	Actor*		GetActor(int iTile) { return m_pTiles[iTile].m_Actor; } //Get the actor at the target tile
	int			GetWidth()	{ return m_iWidth; }
	int			GetHeight()	{ return m_iHeight; }
	

private:
	int				m_iWidth;
	int				m_iHeight;
	kpuVector		m_vCenter;

	sGridTile*		m_pTiles;
};

#define TILE_SIZE 1




