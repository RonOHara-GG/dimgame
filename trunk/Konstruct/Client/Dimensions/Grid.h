#pragma once

#include "Common/Utility/kpuArrayList.h"
#include "Common/Utility/kpuVector.h"
#include "Common/Utility/kpuLinkedList.h"
#include "Common/Utility/kpuGridCord.h"

class Actor;
class GameObject;

class Grid
{
public:
	struct sGridTile
	{
		GameObject*		m_Item;			// One item allowed per tile
		Actor*			m_Actor;			// One actor allowed per tile
	};


	Grid(int iWidth, int iHeight);
	~Grid(void);

	int GetTileAtLocation(const kpuVector& vLocation);
	void GetTileLocation(int iTileIndex, kpuVector& vOutLocation);

	bool BuildPath(int iStartTile, int iEndTile, int* outTiles, int outTilesSize, int iLastDirection = -1);

private:
	int				m_iWidth;
	int				m_iHeight;
	kpuVector		m_vCenter;

	sGridTile*		m_pTiles;
};


