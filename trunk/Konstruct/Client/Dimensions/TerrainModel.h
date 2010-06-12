#pragma once
#include "Common/Graphics/kpgmodel.h"


class TerrainModel :
	public kpgModel
{
public:
	TerrainModel(void);
	~TerrainModel(void);

	bool LoadTerrain(const char* pszFile, int iWidth, int iHeigth);
	kpuFixedArray<kpuPhysicalObject*>* GetPeices() { return m_paPhysicalObjects; }
private:

	kpuFixedArray<kpuPhysicalObject*>*	m_paPhysicalObjects;

	struct TerrainData
	{
		int								iX;
		int								iY;
		int								iRotations;
		char*							szModelFile;
		char*							szCollisionMeshFile;
		kpuVector					    vDimensions;
		int								aWallRanges[8];
		int								aDoorLocations[8];
		int								iPiece;
	};

	struct Space
	{
		int iX;
		int iY;
		int iWidth;
		int iHeight;

		Space(int x = 0, int y= 0, int w= 0, int h= 0) { iX = x; iY = y; iWidth = w; iHeight = h; }
	};

	bool SpacesMatch(int iDoorS1, int iDoorE1, int iDoorS2, int iDoorE2, int iLength);

};
