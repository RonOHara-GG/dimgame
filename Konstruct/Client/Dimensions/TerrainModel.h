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
		float							fX;
		float							fY;
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
		float fX;
		float fY;
		float fWidth;
		float fHeight;

		Space(float x = 0.0f, float y = 0.0f, float w = 0.0f, float h= 0.0f) { fX = x; fY = y; fWidth = w; fHeight = h; }
	};

	bool SpacesMatch(float fDoorS1, float fDoorE1, float fDoorS2, float fDoorE2, float fLength);

};
