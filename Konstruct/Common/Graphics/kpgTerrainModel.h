#pragma once
#include "kpgmodel.h"

class kpgTerrainModel :
	public kpgModel
{
public:
	kpgTerrainModel(void);
	~kpgTerrainModel(void);

	bool LoadTerrain(const char* pszFile, int iWidth, int iHeigth);
private:
	struct TerrainData
	{
		int								iX;
		int								iY;
		int								iRotations;
		char*							szModelFile;
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
