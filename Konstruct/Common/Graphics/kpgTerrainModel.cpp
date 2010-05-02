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
	if (m_paPhysicalObjects)
	{
		for(int i = 0; i < m_paPhysicalObjects->GetNumElements(); i++)
		{
			delete (*m_paPhysicalObjects)[i];
		}

		delete m_paPhysicalObjects;
	}
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
					aTerrainData[iPiece].vDimensions = kpuVector(atof(pEChild->Attribute("Width")), atof(pEChild->Attribute("Height")), atof(pEChild->Attribute("Lenght")), 0.0f);

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
								/*case s_uHash_Wall:
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
									}*/
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
					iSide = ( 4 - iRotation + 3 ) % 4;
				}
				else if( module.iX + module.vDimensions.GetX() == iWidth )
				{
					//check 1
					iSide = ( 4 - iRotation + 1 ) % 4;
				}

				if( iSide > -1 && module.aDoorLocations[iSide * 2] > 1 )
				{
					bValid = false;
					break;
				}

				if( module.iY == 0 )
				{
					//check 0
					iSide = ( 4 - iRotation + 0 ) % 4;
				}
				else if( module.iY + module.vDimensions.GetZ() == iHeigth )
				{
					//check 2
					iSide = ( 4 - iRotation + 2 ) % 4;
				}

				//if opening on side that leads off map find a new piece
				if( iSide > -1 && module.aDoorLocations[iSide * 2] > 1 )
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

					if ( module.iPiece == 0 || module.iPiece == 2 )
						getchar();


					//make sure the doors to this piece don't lead to a wall
					if( iDX > 0 && iDY * iDY <= pTest->vDimensions.GetZ() * pTest->vDimensions.GetZ() && iDX * iDX <= pTest->vDimensions.GetX() * pTest->vDimensions.GetX() )
					{
						//check 1 against 3
						//positive veritcal check
						int iDoorS1 = -1;
						int iDoorE1 = -1;
						int iDoorS2 = -1;
						int iDoorE2 = -1;
						int iDoorLen = -1;
						
						//Get the location of the start of the doors and end of the doors for both pieces
						iSide =  (( 4 - iRotation + 1 ) % 4 ) * 2;
						iDoorS1 = module.aDoorLocations[iSide];
						iDoorE1 = module.aDoorLocations[iSide + 1] - 1 + iDoorS1;

						iSide = (( 4 - pTest->iRotations + 3 ) % 4 ) * 2;
						iDoorS2 = pTest->aDoorLocations[iSide];
						iDoorE2 = pTest->aDoorLocations[iSide + 1] - 1 + iDoorS2;
						iDoorLen = pTest->vDimensions.GetZ();					

						if( iDoorS1 > 0 && iDoorS2 > -1 )
						{
							//+ delta Y since it is positive and vertical
							/*iDoorS1 = ( iDoorLen - 1 ) - iDoorS1 + iDY;
							iDoorE1 = ( iDoorLen - 1 ) - iDoorE1 + iDY;*/

							//Conver the peice being added door point into the test peice's wall space
							iDoorE1 = ( iDoorLen - 1  - iDoorE1 ) + iDY;
							iDoorS1 = ( iDoorLen - 1  - iDoorE1 ) + iDY;

							//make sure the doors are within the test peices doors
							if ( !SpacesMatch(iDoorS1, iDoorE1, iDoorS2, iDoorE2, iDoorLen))
							{
								bValid = false;
								break;
							}
						}

						if( iDoorS2 > 0 && iDoorS1 > -1 )
						{
							//make sure the test peices doors are within the peice being added doors
							if ( !SpacesMatch(iDoorS2, iDoorE2, iDoorS1, iDoorE1, iDoorLen))
							{
								bValid = false;
								break;
							}
						}
					}
					else if( iDX < 0 && iDY * iDY <= pTest->vDimensions.GetZ() * pTest->vDimensions.GetZ() && iDX * iDX <= pTest->vDimensions.GetX() * pTest->vDimensions.GetX())
					{
						//check 3 against 1
						//negative veritcal check
						int iDoorS1 = -1;
						int iDoorE1 = -1;
						int iDoorS2 = -1;
						int iDoorE2 = -1;
						int iDoorLen = -1;						

						//Get the location of the start of the doors and end of the doors for both pieces
						iSide =  (( 4 - iRotation + 3 ) % 4 ) * 2;
						iDoorS1 = module.aDoorLocations[iSide];
						iDoorE1 = module.aDoorLocations[iSide + 1] - 1 + iDoorS1;

						iSide = (( 4 - pTest->iRotations + 1 ) % 4 ) * 2;
						iDoorS2 = pTest->aDoorLocations[iSide];
						iDoorE2 = pTest->aDoorLocations[iSide + 1] - 1 + iDoorS2;
						iDoorLen = pTest->vDimensions.GetZ();						

						if( iDoorS1 > 0 && iDoorS2 > -1 )
						{
							//- delta Y since it is negative and vertical
							/*iDoorS1 = ( iDoorLen - 1 ) - iDoorS1 - iDY;
							iDoorE1 = ( iDoorLen - 1 ) - iDoorE1 - iDY;*/

							//Conver the peice being added door point into the test peice's wall space
							iDoorE1 = ( module.vDimensions.GetZ() - 1 - iDoorE1 ) - iDY;
							iDoorS1 = ( module.vDimensions.GetZ() - 1 - iDoorS1 ) - iDY;

							//make sure the doors are within the test peices doors
							if ( !SpacesMatch(iDoorS1, iDoorE1, iDoorS2, iDoorE2, iDoorLen))
							{
								bValid = false;
								break;
							}
						}

						if( iDoorS2 > 0 && iDoorS1 > -1 )
						{
							//make sure the test peices doors are within the peice being added doors
							if ( !SpacesMatch(iDoorS2, iDoorE2, iDoorS1, iDoorE1, iDoorLen))
							{
								bValid = false;
								break;
							}
						}

					}
					else if( iDY > 0 && iDY * iDY <= pTest->vDimensions.GetZ() * pTest->vDimensions.GetZ() && iDX * iDX <= pTest->vDimensions.GetX() * pTest->vDimensions.GetX() )
					{
						//check 2 against 0
						//negative horizontal check
						int iDoorS1 = -1;
						int iDoorE1 = -1;
						int iDoorS2 = -1;
						int iDoorE2 = -1;
						int iDoorLen = -1;
						
						//Get the location of the start of the doors and end of the doors for both pieces
						iSide =  (( 4 - iRotation + 2 ) % 4 ) * 2;
						iDoorS1 = module.aDoorLocations[iSide];						
						iDoorE1 = module.aDoorLocations[iSide + 1] - 1 + iDoorS1;

						iSide = (( 4 - pTest->iRotations + 0 ) % 4 ) * 2;
						iDoorS2 = pTest->aDoorLocations[iSide];
						iDoorE2 = pTest->aDoorLocations[iSide + 1] - 1 + iDoorS2;
						iDoorLen = pTest->vDimensions.GetX();					

						if( iDoorS1 > 0 && iDoorS2 > -1 )
						{
							//- delta X since it is negative and horizontal
							/*iDoorS1 = ( iDoorLen - 1 ) - iDoorS1 - iDX;
							iDoorE1 = ( iDoorLen - 1 ) - iDoorE1 - iDX;*/

							//Conver the peice being added door point into the test peice's wall space
							iDoorE1 = ( module.vDimensions.GetX() - 1 - iDoorE1 ) - iDX;
							iDoorS1 = ( module.vDimensions.GetX() - 1 - iDoorS1 ) - iDX;

							//make sure the doors are within the test peices doors
							if ( !SpacesMatch(iDoorS1, iDoorE1, iDoorS2, iDoorE2, iDoorLen))
							{
								bValid = false;
								break;
							}
						}

						if( iDoorS2 > 0 && iDoorS1 > -1 )
						{
							//make sure the test peices doors are within the peice being added doors
							if ( !SpacesMatch(iDoorS2, iDoorE2, iDoorS1, iDoorE1, iDoorLen))
							{
								bValid = false;
								break;
							}
						}

					}
					else if( iDY < 0 && iDY * iDY <= pTest->vDimensions.GetZ() * pTest->vDimensions.GetZ() && iDX * iDX <= pTest->vDimensions.GetX() * pTest->vDimensions.GetX() )
					{
						//check 0 against 2
						//positive veritcal check
						int iDoorS1 = -1;
						int iDoorE1 = -1;
						int iDoorS2 = -1;
						int iDoorE2 = -1;
						int iDoorLen = -1;
						
						//Get the location of the start of the doors and end of the doors for both pieces
						iSide =  (( 4 - iRotation + 0 ) % 4 ) * 2;
						iDoorS1 = module.aDoorLocations[iSide];						
						iDoorE1 = module.aDoorLocations[iSide + 1] - 1 + iDoorS1;

						iSide = (( 4 - pTest->iRotations + 2 ) % 4 ) * 2;
						iDoorS2 = pTest->aDoorLocations[iSide];
						iDoorE2 = pTest->aDoorLocations[iSide + 1] - 1 + iDoorS2;
						iDoorLen = pTest->vDimensions.GetX();
						
						if( iDoorS1 > 0 && iDoorS2 > -1 )
						{
							//+ delta X since it is positive and Horzontal
							/*iDoorS1 = ( iDoorLen - 1 ) - iDoorS1 + iDX;
							iDoorE1 = ( iDoorLen - 1 ) - iDoorE1 + iDX;*/

							//Conver the peice being added door point into the test peice's wall space
							iDoorE1 = ( pTest->vDimensions.GetX() - 1 - iDoorE1 ) + iDX;
							iDoorS1 = ( pTest->vDimensions.GetX() - 1 - iDoorS1 ) + iDX;

							//make sure the doors are within the test peices doors
							if ( !SpacesMatch(iDoorS1, iDoorE1, iDoorS2, iDoorE2, iDoorLen))
							{
								bValid = false;
								break;
							}
						}

						if( iDoorS2 > 0 && iDoorS1 > -1 )
						{
							//make sure the test peices doors are within the peice being added doors
							if ( !SpacesMatch(iDoorS2, iDoorE2, iDoorS1, iDoorE1, iDoorLen))
							{
								bValid = false;
								break;
							}
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

	m_paPhysicalObjects = new kpuFixedArray<kpuPhysicalObject*>(aFinalMap.Count());

	//add all instances to this model
	for(int i = 0; i < aFinalMap.Count(); i++)
	{		
		//make new instance
		kpgGeometryInstance* pInst = new kpgGeometryInstance(m_aGeometries[aFinalMap[i].iPiece]);

		pInst->SetPermYRotation( aFinalMap[i].iRotations * -1.570796 );
		kpuVector vPos(aFinalMap[i].iX - iWidth / 2 + aFinalMap[i].vDimensions.GetX() / 2, 0.0f , aFinalMap[i].iY  - iHeigth / 2+ aFinalMap[i].vDimensions.GetZ() / 2, 0.0f);
		pInst->SetPosition(vPos.GetX(), vPos.GetY(), vPos.GetZ());
		
		m_aInstances.Add(pInst);

		vPos += ( aFinalMap[i].vDimensions * -0.5 );
		vPos.SetY(0);
	
		//make a new physical object and set its collision mesh
		kpuPhysicalObject* obj = new kpuPhysicalObject(vPos, vPos + aFinalMap[i].vDimensions);
		kpgModel* collisionMesh = new kpgModel();
		collisionMesh->SetGeometryInstance(pInst);
		obj->SetCollisionMesh(collisionMesh);

		m_paPhysicalObjects->Add(obj);	
	}

	return true;
}

bool kpgTerrainModel::SpacesMatch(int iDoorS1, int iDoorE1, int iDoorS2, int iDoorE2, int iLength)
{
	if( ( iDoorS1 >= 0 && iDoorS1 < iLength ) && ( iDoorS1 < iDoorS2 || iDoorS1 > iDoorE2 ) )
		return false;

	if( ( iDoorE1 >= 0 && iDoorE1 < iLength ) && ( iDoorE1 < iDoorS2 || iDoorE1 > iDoorE2 ) )
		return false;

	return true;
}
