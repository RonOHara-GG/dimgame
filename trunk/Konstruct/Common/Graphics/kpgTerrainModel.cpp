#include "StdAfx.h"
#include "Common/Graphics/kpgGeometry.h"
#include "Common/Graphics/kpgGeometryInstance.h"
#include "Common/Utility/kpuFixedArray.h"
#include "Common/Utility/kpuArrayList.h"
#include "Common/Utility/kpuLinkedList.h"
#include "kpgTerrainModel.h"
#include "Common/Utility/kpuFileManager.h"
#include "External/tinyxml/tinyxml.h"

static const u32 s_uHash_Door =		0x7c844039;
static const u32 s_uHash_Wall =		0x7c8e6f75;
static const u32 s_uHash_Data =		0x7c84053f;
static const u32 s_uHash_Top =		0xb8826f8;
static const u32 s_uHash_Left =		0x7c887790;
static const u32 s_uHash_Right =	0xe063143;
static const u32 s_uHash_Bottom =	0xa9edb65a;




kpgTerrainModel::kpgTerrainModel(void)
{
}

kpgTerrainModel::~kpgTerrainModel(void)
{
}

bool kpgTerrainModel::LoadTerrain(const char* pszFile, int iWidth, int iHeigth)
{
	kpuFixedArray<TerrainData> aTerrainData;

	TiXmlDocument doc;

	char szFileName[2048];
	kpuFileManager::GetFullFilePath(pszFile, szFileName, sizeof(szFileName));

	//Load all peices
	if( doc.LoadFile(szFileName) )
	{
		for(TiXmlElement* pEStart = doc.FirstChildElement(); pEStart != 0; pEStart = pEStart->NextSiblingElement() )
		{
			int iCount = atoi(pEStart->Attribute("Count"));  

			
			aTerrainData.SetSize(iCount);

			int iPiece = 0;

			for( TiXmlElement* pEChild = pEStart->FirstChildElement(); pEChild != 0; pEChild = pEChild->NextSiblingElement() )
			{
				u32 uHash = StringHash(pEChild->Value());
				if( uHash == s_uHash_Data )
				{					
					aTerrainData[iPiece].szModelFile = (char*)pEChild->Attribute("File");
					aTerrainData[iPiece].vDimensions = kpuVector(atof(pEChild->Attribute("Width")), 0.0f, atof(pEChild->Attribute("Height")), 0.0f);

					for( TiXmlElement* pEChild2 = pEChild->FirstChildElement(); pEChild2 != 0; pEChild2 = pEChild2->NextSiblingElement() )
					{				
							uHash = StringHash(pEChild2->Value());
							
							switch(uHash)
							{
								case s_uHash_Door:
									{
										uHash = StringHash(pEChild2->Attribute("Side"));
										switch(uHash)
										{
											case s_uHash_Top:
												{
													char* szData = (char*)pEChild2->FirstChild()->Value();

													aTerrainData[iPiece].aDoorLocations[0] = atoi(&szData[0]);

													while(*szData && *szData != ' ') szData++;
													szData++;

													aTerrainData[iPiece].aDoorLocations[1] = atoi(&szData[0]);
													break;
												}
											case s_uHash_Right:
												{
													char* szData = (char*)pEChild2->FirstChild()->Value();

													aTerrainData[iPiece].aDoorLocations[2] = atoi(&szData[0]);

													while(*szData && *szData != ' ') szData++;
													szData++;

													aTerrainData[iPiece].aDoorLocations[3] = atoi(&szData[0]);
													break;
												}
											case s_uHash_Bottom:
												{
													char* szData = (char*)pEChild2->FirstChild()->Value();

													aTerrainData[iPiece].aDoorLocations[4] = atoi(&szData[0]);

													while(*szData && *szData != ' ') szData++;
													szData++;

													aTerrainData[iPiece].aDoorLocations[5] = atoi(&szData[0]);
													break;
												}
											case s_uHash_Left:
												{
													char* szData = (char*)pEChild2->FirstChild()->Value();

													aTerrainData[iPiece].aDoorLocations[6] = atoi(&szData[0]);

													while(*szData && *szData != ' ') szData++;
													szData++;

													aTerrainData[iPiece].aDoorLocations[7] = atoi(&szData[0]);
													break;
												}
										}
										break;
									}
								case s_uHash_Wall:
									{
										uHash = StringHash( pEChild2->Attribute("Side"));
										switch(uHash)
										{
											case s_uHash_Top:
												{
													char* szData = (char*)pEChild2->FirstChild()->Value();

													aTerrainData[iPiece].aWallRanges[0] = atoi(&szData[0]);

													while(*szData && *szData != ' ') szData++;
													szData++;

													aTerrainData[iPiece].aWallRanges[1] = atoi(&szData[0]);
													break;
												}
											case s_uHash_Right:
												{
													char* szData = (char*)pEChild2->FirstChild()->Value();

													aTerrainData[iPiece].aWallRanges[2] = atoi(&szData[0]);

													while(*szData && *szData != ' ') szData++;
													szData++;

													aTerrainData[iPiece].aWallRanges[3] = atoi(&szData[0]);
													break;
												}
											case s_uHash_Bottom:
												{
													char* szData = (char*)pEChild2->FirstChild()->Value();

													aTerrainData[iPiece].aWallRanges[4] = atoi(&szData[0]);

													while(*szData && *szData != ' ') szData++;
													szData++;

													aTerrainData[iPiece].aWallRanges[5] = atoi(&szData[0]);
													break;
												}
											case s_uHash_Left:
												{
													char* szData = (char*)pEChild2->FirstChild()->Value();

													aTerrainData[iPiece].aWallRanges[6] = atoi(&szData[0]);

													while(*szData && *szData != ' ') szData++;
													szData++;

													aTerrainData[iPiece].aWallRanges[7] = atoi(&szData[0]);
													break;
												}
										}
										break;
									}
							}							
					}

					iPiece++;
				}
			}
		}
	}
	else
		return false;


	//Now fit all the peices together

	
	kpuArrayList<TerrainData> aFinalMap;
	kpuLinkedList spacesAvailable;
	
	Space* space = new Space(0, 0,iWidth, iHeigth);

	spacesAvailable.Insert(space);
	

	//random rotation
	while( spacesAvailable.Next() )
	{
		int iPiece = rand() % aTerrainData.GetNumElements();
		TerrainData module = aTerrainData[iPiece];

		int iRotation = rand() % 4;		

		if( iRotation & 1 )
		{
			//flip dimensions
			float xtemp = module.vDimensions.GetX();
			module.vDimensions.SetX(module.vDimensions.GetZ());
			module.vDimensions.SetZ(xtemp);
		}

		kpuLinkedList* pIter = spacesAvailable.Next();

		while(pIter && pIter->GetPointer() )
		{	

			//see if object fits in space
			Space* space = (Space*)pIter->GetPointer();

			if( space->iHeight >= module.vDimensions.GetZ() && space->iWidth >= module.vDimensions.GetX() )
			{

				//add peice to the space and divide the space
				module.iPiece = iPiece;
				module.iRotations = iRotation;
				module.iX = space->iX;
				module.iY = space->iY;

				bool bValid = true;

				//Make sure doors doesn't lead to no where
				int iDoor = -1;
				int iSide = -1;

				if( module.iX == 0 )
				{
					//check 3
					iSide = abs(3 - iRotation);
				}
				else if( module.iX + module.vDimensions.GetX() == iWidth )
				{
					//check 1
					iSide = abs(1 - iRotation);
				}

				if( iSide > 0 && module.aDoorLocations[iSide] > 0 )
				{
					bValid = false;
					break;
				}

				if( module.iY == 0 )
				{
					//check 0
					iSide = abs(0 - iRotation);
				}
				else if( module.iY + module.vDimensions.GetZ() == iHeigth )
				{
					//check 2
					iSide = abs(2 - iRotation);
				}

				if( iSide > 0 && module.aDoorLocations[iSide] > 0 )
				{
					bValid = false;
					break;
				}


				//Check this peices doors agianst the rest of the peices
				for(int i =0; i < aFinalMap.Count(); i++)
				{
					TerrainData* pTest = &aFinalMap[i];

					int iDX = pTest->iX - module.iX;
					int iDY = pTest->iY - module.iY;

					int iDoorS = -1;
					int iDoorE = -1;

					int iWallS = -1;
					int iWallE = -1;

					if( iDX <= module.vDimensions.GetX() && iDY * iDY < module.vDimensions.GetZ() * module.vDimensions.GetZ() )
					{
						//check 1 against 3
						iSide = abs(1 - iRotation) * 2;
						iDoorS = module.aDoorLocations[iSide];
						iDoorE = module.aDoorLocations[iSide + 1]  + iDoorS - 1;

						iSide = abs(3 - pTest->iRotations) * 2;
						iWallS = pTest->aWallRanges[iSide];
						iWallE = pTest->aWallRanges[iSide + 1];	

						iDoorS = iWallE - iDoorS + iDY;
						iDoorE = iWallE - iDoorE + iDY;
							

					}
					else if( iDX >= -module.vDimensions.GetX() && iDY * iDY < module.vDimensions.GetZ() * module.vDimensions.GetZ() )
					{
						//check 3 against 1
						iSide = abs(3 - iRotation) * 2;
						iDoorS = module.aDoorLocations[iSide];
						iDoorE = module.aDoorLocations[iSide + 1]  + iDoorS - 1;

						iSide = abs(1 - pTest->iRotations) * 2;
						iWallS = pTest->aWallRanges[iSide];
						iWallE = pTest->aWallRanges[iSide + 1];

						iDoorS = iWallE - iDoorS + iDY;
						iDoorE = iWallE - iDoorE + iDY;

					}
					else if( iDY <= module.vDimensions.GetZ() && iDX * iDX < module.vDimensions.GetX() * module.vDimensions.GetX() )
					{
						//check 2 against 0
						iSide = abs(2 - iRotation) * 2;
						iDoorS = module.aDoorLocations[iSide];
						iDoorE = module.aDoorLocations[iSide + 1]  + iDoorS - 1;

						iSide = abs(0 - pTest->iRotations) * 2;
						iWallS = pTest->aWallRanges[iSide];
						iWallE = pTest->aWallRanges[iSide + 1];

						iDoorS = iWallE - iDoorS + iDX;
						iDoorE = iWallE - iDoorE + iDX;

					}
					else if( iDY >= -module.vDimensions.GetZ() && iDX * iDX < module.vDimensions.GetX() * module.vDimensions.GetX() )
					{
						//check 0 against 2
						iSide = abs(0 - iRotation) * 2;
						iDoorS = module.aDoorLocations[iSide];
						iDoorE = module.aDoorLocations[iSide + 1]  + iDoorS - 1;

						iSide = abs(2 - pTest->iRotations) * 2;
						iWallS = pTest->aWallRanges[iSide];
						iWallE = pTest->aWallRanges[iSide + 1];

						iDoorS = iWallE - iDoorS + iDX;
						iDoorE = iWallE - iDoorE + iDX;

					}

					if( iDoorS > 0 && iWallS >= 0)
						{
							if (iDoorS > iWallS && iDoorS < iWallE )
							{
								bValid = false;
								break;
							}

							if (iDoorE > iWallS && iDoorE < iWallE )
							{
								bValid = false;
								break;
							}
						}
					

				}
			
				if(bValid)
				{
					aFinalMap.Add(module);

					Space* space1 = new Space( space->iX, space->iY + module.vDimensions.GetZ(), module.vDimensions.GetX(), iHeigth - module.iY - module.vDimensions.GetZ() );
					
					space->iWidth = space->iWidth - module.vDimensions.GetX();
					space->iX = module.iX + module.vDimensions.GetX();

					pIter->InsertBefore(space1);

					if( space1->iHeight < 1 )
					{
						delete space1;
						delete pIter->Prev();
					}

					if(space->iWidth < 1 )
					{
						delete space;
						delete pIter;
					}

					break;
				}

			}

			//Find a space and try to fill it
			pIter = pIter->Next();
		}
	}

	m_aGeometries.SetSize(aTerrainData.GetNumElements());

	//Load geometries
	for(int i = 0; i < aTerrainData.GetNumElements(); i++)
	{
		kpgModel* model = new kpgModel();

		model->Load(aTerrainData[i].szModelFile);
		
		m_aGeometries.Add(model->GetInstance(0)->GetGeometry());
	}

	m_aInstances.SetSize(aFinalMap.Count());
	//add all instances to this model
	for(int i = 0; i < aFinalMap.Count(); i++)
	{		
		//make new instance
		kpgGeometryInstance* pInst = new kpgGeometryInstance(m_aGeometries[aFinalMap[i].iPiece]);

		pInst->SetPermYRotation( aFinalMap[i].iRotations * -1.570796 );
		pInst->SetPosition( aFinalMap[i].iX - iWidth / 2 + aFinalMap[i].vDimensions.GetX() / 2, 0.0f , aFinalMap[i].iY  - iHeigth / 2+ aFinalMap[i].vDimensions.GetZ() / 2);
		
		m_aInstances.Add(pInst);
	}

	return true;
}
