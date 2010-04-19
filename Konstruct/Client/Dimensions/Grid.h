#pragma once

#include "Common/Utility/kpuArrayList.h"
#include "Common/Utility/kpuVector.h"
#include "Common/Utility/kpuLinkedList.h"
#include "Common/Utility/kpuGridCord.h"
#include "Common/Graphics/kpgModel.h"

class Actor;
class GameObject;

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

	int			GetTileAtLocation(const kpuVector& vLocation);
	void		GetTileLocation(int iTileIndex, kpuVector& vOutLocation);
	bool		AddActor(Actor* pActor); //Add actor to the gird, if an actor is already on the tile return false else return ture
	bool		RemoveActor(Actor* pActor);//Remove actor from his current tile, if he is not found on the current tile return false
	bool		IsWalkable(int iTile); //Determines if a tile is walkable or not

	bool		BuildPath(int iStartTile, int& iEndTile, int* outTiles, int outTilesSize, int iLastDirection = -1);
	bool		AddTerrain(kpgModel* pTerrain); //Add a piece of terrain to the grid if tiles not occupied
	GameObject* GetLoot(int iTile) { return m_pTiles[iTile].m_Item; } //Get the loot at the target tile
	Actor*		GetActor(int iTile) { return m_pTiles[iTile].m_Actor; } //Get the actor at the target tile
	kpuVector	GetDimensions() { return kpuVector(m_iWidth, 0, m_iHeight, 0); }
	

private:
	int				m_iWidth;
	int				m_iHeight;
	kpuVector		m_vCenter;

	sGridTile*		m_pTiles;
};




