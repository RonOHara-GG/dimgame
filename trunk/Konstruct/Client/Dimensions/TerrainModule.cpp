#include "StdAfx.h"
#include "TerrainModule.h"
#include "External/tinyxml/tinyxml.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Graphics/kpgRenderer.h"

static const u32 s_uHash_Doors =		0xd0c47cc;
static const u32 s_uHash_Walls =		0xe5c5e88;

TerrainModule::TerrainModule()
{
	m_iRotations = 0;
	m_pModel = 0;
	m_aDoorLocations = new kpuArrayList<kpuVector>();
	m_aWallRanges = new kpuArrayList<kpuVector>();
	
}

TerrainModule::TerrainModule(TerrainModule* pOriginal)
{
	m_pModel = new kpgModel();
	m_pModel->Load(pOriginal->m_pszModelFile);

	m_aDoorLocations = new kpuArrayList<kpuVector>();
	m_aWallRanges = new kpuArrayList<kpuVector>();

	for(int i = 0; i < pOriginal->m_aDoorLocations->Count(); i++)
	{
		kpuVector vPos = (*pOriginal->m_aDoorLocations)[i];
		m_aDoorLocations->Add(vPos);
	}

	for(int i = 0; i < pOriginal->m_aWallRanges->Count(); i++)
	{
		kpuVector vPos = (*pOriginal->m_aWallRanges)[i];
		m_aWallRanges->Add(vPos);		
	}

	m_iRotations = pOriginal->m_iRotations;
	m_vDimensions = pOriginal->m_vDimensions;
	m_pszModelFile = pOriginal->m_pszModelFile;
}

TerrainModule::~TerrainModule(void)
{
	delete m_pModel;
	delete m_aDoorLocations;
	delete m_aWallRanges;
}

kpuVector TerrainModule::GetPosition()
{
	return m_pModel->GetPosition();
}

bool TerrainModule::Load(TiXmlElement* pEData)
{


	m_pModel = new kpgModel();
	m_pszModelFile = (char*)pEData->Attribute("File");
	m_pModel->Load(m_pszModelFile);

	m_vDimensions.SetX(atof(pEData->Attribute("Width")));
	m_vDimensions.SetY(0.0f);
	m_vDimensions.SetZ(atof(pEData->Attribute("Height")));

	for(TiXmlElement* pEChild = pEData->FirstChildElement(); pEChild != 0; pEChild = pEChild->NextSiblingElement())
	{
		int uHash = StringHash(pEChild->Value());

		switch (uHash)
		{
			case s_uHash_Doors:
				{
					int iCount = atoi(pEChild->Attribute("Count"));

					if( iCount > 0 )
					{
						char* pszDoors = _strdup(pEChild->FirstChild()->Value());

						for(int i = 0; i < iCount; i++)
						{
							kpuVector vDoor;

							vDoor.SetX(atof(&pszDoors[0]));

							while( *pszDoors && *pszDoors != ' ') pszDoors++;
							pszDoors++;

							vDoor.SetY(atof(&pszDoors[0]));

							while( *pszDoors && *pszDoors != ' ') pszDoors++;
							pszDoors++;

							vDoor.SetZ(atof(&pszDoors[0]));

							while( *pszDoors && *pszDoors != ' ') pszDoors++;
							pszDoors++;

							vDoor.SetW(atof(&pszDoors[0]));						
							
							while( *pszDoors && *pszDoors != ' ') pszDoors++;
							pszDoors++;

							m_aDoorLocations->Add(vDoor);
						}
					}

					break;
				}
			case s_uHash_Walls:
				{
					int iCount = atoi(pEChild->Attribute("Count"));

					if( iCount > 0 )
					{
						char* pszWalls = _strdup(pEChild->FirstChild()->Value());

						for(int i = 0; i < iCount; i++)
						{
							kpuVector vWall;

							vWall.SetX(atof(&pszWalls[0])); 

							while(*pszWalls && *pszWalls != ' ') pszWalls++;
							pszWalls++;

							vWall.SetY(atof(&pszWalls[0]));

							while(*pszWalls && *pszWalls != ' ') pszWalls++;
							pszWalls++;

							vWall.SetZ(atof(&pszWalls[0])); //This is the start location of the wall

							while(*pszWalls && *pszWalls != ' ') pszWalls++;
							pszWalls++;

							vWall.SetW(atof(&pszWalls[0])); //This is how many tiles it spans						
							
							while(*pszWalls && *pszWalls != ' ') pszWalls++;
							pszWalls++;

							m_aWallRanges->Add(vWall);

						}
					}

					break;


				}
		}
	}

	return true;
}

TerrainModule* TerrainModule::CreateCopy()
{
	return new TerrainModule(this);
}

void TerrainModule::RotateClockWise(int iRotations)
{
	m_iRotations = iRotations;

	while (iRotations > 0 )
	{
		//Rotate the normal of each door and wall range
		//All rotations will be 90 degrees clockwise

		for(int i = 0; i < m_aDoorLocations->Count(); i++)
		{
			kpuVector* vPos = &(*m_aDoorLocations)[i];

			float fXTemp = vPos->GetX();

			vPos->SetX(-vPos->GetZ() + 1);
			vPos->SetZ(fXTemp);
		}

		for(int i = 0; i < m_aWallRanges->Count(); i++)
		{
			kpuVector* vPos = &(*m_aWallRanges)[i];

			float fXTemp = vPos->GetX();

			vPos->SetX(-vPos->GetY() + 1);
			vPos->SetY(fXTemp);
		}


		//flip dimensions
		float fXTemp = m_vDimensions.GetX();

		m_vDimensions.SetX( m_vDimensions.GetZ() );
		m_vDimensions.SetZ( fXTemp );

		iRotations--;

	}

	
	



}

void TerrainModule::Draw(kpgRenderer *pRenderer)
{
	m_pModel->RotateY(-1.5707 * m_iRotations);
	m_pModel->Draw(pRenderer);
}