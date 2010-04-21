#pragma once
#include "Common/Utility/kpuArrayList.h"
#include "Common/Utility/kpuVector.h"

class TiXmlElement;
class kpgModel;
class kpgRenderer;

/*
*	The door vector is in the following format: X,Y,Z hold the corner that the door is refrenced from.  W is the |offset| from that corner to where the door is
*	The wall vector is in the following format: X,Y hold the corner the door is reference from. Z is the |offset| from that corner to the start of the wall
*	and W holds how far from Z the wall spans
*/

class TerrainModule
{
public:
	TerrainModule(void);
	TerrainModule( TerrainModule* pOriginal);
	~TerrainModule(void);

	
	kpgModel*					GetModel()    { return m_pModel; }
	bool						Load(TiXmlElement* pEData);	
	TerrainModule*				CreateCopy();
	kpuVector					GetDimensions() { return m_vDimensions; }
	kpuVector					GetPosition();
	kpuArrayList<kpuVector>*	GetDoors()		{ return m_aDoorLocations; }
	kpuArrayList<kpuVector>*	GetWalls()		{ return m_aWallRanges; }

	void RotateClockWise(int iRotations);

	void Draw( kpgRenderer* pRenderer );

protected:
	int								m_iRotations;
	char*							m_pszModelFile;
	kpgModel*						m_pModel;
	kpuVector					    m_vDimensions;
	kpuArrayList<kpuVector>*		m_aWallRanges;
	kpuArrayList<kpuVector>*		m_aDoorLocations;
};
