#include "StdAfx.h"
#include "Common/Graphics/kpgGeometry.h"
#include "Common/Graphics/kpgGeometryInstance.h"
#include "Common/Utility/kpuFixedArray.h"
#include "Common/Utility/kpuArrayList.h"
#include "Common/Utility/kpuLinkedList.h"
#include "TerrainModel.h"
#include "Common/Utility/kpuFileManager.h"
#include "External/tinyxml/tinyxml.h"
#include "GameObject.h"

static const u32 s_uHash_Door =		0x7c844039;
static const u32 s_uHash_Wall =		0x7c8e6f75;
static const u32 s_uHash_Data =		0x7c84053f;
static const u32 s_uHash_Top =		0xb8826f8;
static const u32 s_uHash_Left =		0x7c887790;
static const u32 s_uHash_Right =	0xe063143;
static const u32 s_uHash_Bottom =	0xa9edb65a;




TerrainModel::TerrainModel(void)
{
}

TerrainModel::~TerrainModel(void)
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

bool TerrainModel::LoadTerrain(const char* pszFile, int fWidth, int iHeigth)
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
					aTerrainData[iPiece].szCollisionMeshFile = (char*)pEChild->Attribute("Collision");

					if( strlen(aTerrainData[iPiece].szCollisionMeshFile) < 1 )
						aTerrainData[iPiece].szCollisionMeshFile = 0;

					aTerrainData[iPiece].vDimensions = kpuVector((float)atof(pEChild->Attribute("Width")), (float)atof(pEChild->Attribute("Height")), (float)atof(pEChild->Attribute("Lenght")), 0.0f);

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
	
	Space* space = new Space(0, 0,fWidth, iHeigth);

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

			if( space->fHeight >= module.vDimensions.GetZ() && space->fWidth >= module.vDimensions.GetX() )
			{
				//add peice to the space and divide the space
				module.iPiece = iPiece;
				module.iRotations = iRotation;
				module.fX = space->fX;
				module.fY = space->fY;

				bool bValid = true;

				//Make sure doors doesn't lead to no where
				int fDoor = -1;
				int iSide = -1;

				if( module.fX == 0 )
				{
					//check 3
					iSide = ( 4 - iRotation + 3 ) % 4;
				}
				else if( module.fX + module.vDimensions.GetX() == fWidth )
				{
					//check 1
					iSide = ( 4 - iRotation + 1 ) % 4;
				}

				if( iSide > -1 && module.aDoorLocations[iSide * 2] > 1 )
				{
					bValid = false;
					break;
				}

				if( module.fY == 0 )
				{
					//check 0
					iSide = ( 4 - iRotation + 0 ) % 4;
				}
				else if( module.fY + module.vDimensions.GetZ() == iHeigth )
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

					float fDX = pTest->fX - module.fX;
					float fDY = pTest->fY - module.fY;	

					if ( module.iPiece == 0 || module.iPiece == 2 )
						getchar();


					//make sure the doors to this piece don't lead to a wall
					if( fDX > 0 && fDY * fDY <= pTest->vDimensions.GetZ() * pTest->vDimensions.GetZ() && fDX * fDX <= pTest->vDimensions.GetX() * pTest->vDimensions.GetX() )
					{
						//check 1 against 3
						//positive veritcal check
						float fDoorS1 = -1;
						float fDoorE1 = -1;
						float fDoorS2 = -1;
						float fDoorE2 = -1;
						float fDoorLen = -1;
						
						//Get the location of the start of the doors and end of the doors for both pieces
						iSide =  (( 4 - iRotation + 1 ) % 4 ) * 2;
						fDoorS1 = module.aDoorLocations[iSide];
						fDoorE1 = module.aDoorLocations[iSide + 1] - 1 + fDoorS1;

						iSide = (( 4 - pTest->iRotations + 3 ) % 4 ) * 2;
						fDoorS2 = pTest->aDoorLocations[iSide];
						fDoorE2 = pTest->aDoorLocations[iSide + 1] - 1 + fDoorS2;
						fDoorLen = pTest->vDimensions.GetZ();					

						if( fDoorS1 > 0 && fDoorS2 > -1 )
						{
							//+ delta Y since it is positive and vertical
							/*fDoorS1 = ( fDoorLen - 1 ) - fDoorS1 + fDY;
							fDoorE1 = ( fDoorLen - 1 ) - fDoorE1 + fDY;*/

							//Conver the peice being added door point into the test peice's wall space
							fDoorE1 = ( fDoorLen - 1  - fDoorE1 ) + fDY;
							fDoorS1 = ( fDoorLen - 1  - fDoorE1 ) + fDY;

							//make sure the doors are within the test peices doors
							if ( !SpacesMatch(fDoorS1, fDoorE1, fDoorS2, fDoorE2, fDoorLen))
							{
								bValid = false;
								break;
							}
						}

						if( fDoorS2 > 0 && fDoorS1 > -1 )
						{
							//make sure the test peices doors are within the peice being added doors
							if ( !SpacesMatch(fDoorS2, fDoorE2, fDoorS1, fDoorE1, fDoorLen))
							{
								bValid = false;
								break;
							}
						}
					}
					else if( fDX < 0 && fDY * fDY <= pTest->vDimensions.GetZ() * pTest->vDimensions.GetZ() && fDX * fDX <= pTest->vDimensions.GetX() * pTest->vDimensions.GetX())
					{
						//check 3 against 1
						//negative veritcal check
						float fDoorS1 = -1;
						float fDoorE1 = -1;
						float fDoorS2 = -1;
						float fDoorE2 = -1;
						float fDoorLen = -1;						

						//Get the location of the start of the doors and end of the doors for both pieces
						iSide =  (( 4 - iRotation + 3 ) % 4 ) * 2;
						fDoorS1 = module.aDoorLocations[iSide];
						fDoorE1 = module.aDoorLocations[iSide + 1] - 1 + fDoorS1;

						iSide = (( 4 - pTest->iRotations + 1 ) % 4 ) * 2;
						fDoorS2 = pTest->aDoorLocations[iSide];
						fDoorE2 = pTest->aDoorLocations[iSide + 1] - 1 + fDoorS2;
						fDoorLen = pTest->vDimensions.GetZ();						

						if( fDoorS1 > 0 && fDoorS2 > -1 )
						{
							//- delta Y since it is negative and vertical
							/*fDoorS1 = ( fDoorLen - 1 ) - fDoorS1 - fDY;
							fDoorE1 = ( fDoorLen - 1 ) - fDoorE1 - fDY;*/

							//Conver the peice being added door point into the test peice's wall space
							fDoorE1 = ( module.vDimensions.GetZ() - 1 - fDoorE1 ) - fDY;
							fDoorS1 = ( module.vDimensions.GetZ() - 1 - fDoorS1 ) - fDY;

							//make sure the doors are within the test peices doors
							if ( !SpacesMatch(fDoorS1, fDoorE1, fDoorS2, fDoorE2, fDoorLen))
							{
								bValid = false;
								break;
							}
						}

						if( fDoorS2 > 0 && fDoorS1 > -1 )
						{
							//make sure the test peices doors are within the peice being added doors
							if ( !SpacesMatch(fDoorS2, fDoorE2, fDoorS1, fDoorE1, fDoorLen))
							{
								bValid = false;
								break;
							}
						}

					}
					else if( fDY > 0 && fDY * fDY <= pTest->vDimensions.GetZ() * pTest->vDimensions.GetZ() && fDX * fDX <= pTest->vDimensions.GetX() * pTest->vDimensions.GetX() )
					{
						//check 2 against 0
						//negative horizontal check
						float fDoorS1 = -1;
						float fDoorE1 = -1;
						float fDoorS2 = -1;
						float fDoorE2 = -1;
						float fDoorLen = -1;
						
						//Get the location of the start of the doors and end of the doors for both pieces
						iSide =  (( 4 - iRotation + 2 ) % 4 ) * 2;
						fDoorS1 = module.aDoorLocations[iSide];						
						fDoorE1 = module.aDoorLocations[iSide + 1] - 1 + fDoorS1;

						iSide = (( 4 - pTest->iRotations + 0 ) % 4 ) * 2;
						fDoorS2 = pTest->aDoorLocations[iSide];
						fDoorE2 = pTest->aDoorLocations[iSide + 1] - 1 + fDoorS2;
						fDoorLen = pTest->vDimensions.GetX();					

						if( fDoorS1 > 0 && fDoorS2 > -1 )
						{
							//- delta X since it is negative and horizontal
							/*fDoorS1 = ( fDoorLen - 1 ) - fDoorS1 - fDX;
							fDoorE1 = ( fDoorLen - 1 ) - fDoorE1 - fDX;*/

							//Conver the peice being added door point into the test peice's wall space
							fDoorE1 = ( module.vDimensions.GetX() - 1 - fDoorE1 ) - fDX;
							fDoorS1 = ( module.vDimensions.GetX() - 1 - fDoorS1 ) - fDX;

							//make sure the doors are within the test peices doors
							if ( !SpacesMatch(fDoorS1, fDoorE1, fDoorS2, fDoorE2, fDoorLen))
							{
								bValid = false;
								break;
							}
						}

						if( fDoorS2 > 0 && fDoorS1 > -1 )
						{
							//make sure the test peices doors are within the peice being added doors
							if ( !SpacesMatch(fDoorS2, fDoorE2, fDoorS1, fDoorE1, fDoorLen))
							{
								bValid = false;
								break;
							}
						}

					}
					else if( fDY < 0 && fDY * fDY <= pTest->vDimensions.GetZ() * pTest->vDimensions.GetZ() && fDX * fDX <= pTest->vDimensions.GetX() * pTest->vDimensions.GetX() )
					{
						//check 0 against 2
						//positive veritcal check
						float fDoorS1 = -1;
						float fDoorE1 = -1;
						float fDoorS2 = -1;
						float fDoorE2 = -1;
						float fDoorLen = -1;
						
						//Get the location of the start of the doors and end of the doors for both pieces
						iSide =  (( 4 - iRotation + 0 ) % 4 ) * 2;
						fDoorS1 = module.aDoorLocations[iSide];						
						fDoorE1 = module.aDoorLocations[iSide + 1] - 1 + fDoorS1;

						iSide = (( 4 - pTest->iRotations + 2 ) % 4 ) * 2;
						fDoorS2 = pTest->aDoorLocations[iSide];
						fDoorE2 = pTest->aDoorLocations[iSide + 1] - 1 + fDoorS2;
						fDoorLen = pTest->vDimensions.GetX();
						
						if( fDoorS1 > 0 && fDoorS2 > -1 )
						{
							//+ delta X since it is positive and Horzontal
							/*fDoorS1 = ( fDoorLen - 1 ) - fDoorS1 + fDX;
							fDoorE1 = ( fDoorLen - 1 ) - fDoorE1 + fDX;*/

							//Conver the peice being added door point into the test peice's wall space
							fDoorE1 = ( pTest->vDimensions.GetX() - 1 - fDoorE1 ) + fDX;
							fDoorS1 = ( pTest->vDimensions.GetX() - 1 - fDoorS1 ) + fDX;

							//make sure the doors are within the test peices doors
							if ( !SpacesMatch(fDoorS1, fDoorE1, fDoorS2, fDoorE2, fDoorLen))
							{
								bValid = false;
								break;
							}
						}

						if( fDoorS2 > 0 && fDoorS1 > -1 )
						{
							//make sure the test peices doors are within the peice being added doors
							if ( !SpacesMatch(fDoorS2, fDoorE2, fDoorS1, fDoorE1, fDoorLen))
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

					Space* space1 = new Space( space->fX, space->fY + module.vDimensions.GetZ(), module.vDimensions.GetX(), iHeigth - module.fY - module.vDimensions.GetZ() );
					
					space->fWidth = space->fWidth - module.vDimensions.GetX();
					space->fX = module.fX + module.vDimensions.GetX();

					pIter->InsertBefore(space1);

					if( space1->fHeight < 1 )
					{
						delete space1;
						delete pIter->Prev();
					}

					if(space->fWidth < 1 )
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

	/*TerrainData data = aTerrainData[0];
	data.iPiece = 0;
	data.iRotations = 0;
	data.fX = 50;
	data.fY = 50;
	aFinalMap.Add(data);*/


	m_aGeometries.SetSize(aTerrainData.GetNumElements());
	kpuFixedArray<kpgModel*> aCollisionMeshes(aTerrainData.GetNumElements());

	//Load geometries
	for(int i = 0; i < aTerrainData.GetNumElements(); i++)
	{
		kpgModel* model = new kpgModel();

		model->Load(aTerrainData[i].szModelFile);
		m_aGeometries.Add(model->GetInstance(0)->GetGeometry());
		
		model = new kpgModel();
		model->Load(aTerrainData[i].szCollisionMeshFile);
		aCollisionMeshes.Add(model);		
	}

	m_aInstances.SetSize(aFinalMap.Count());

	m_paPhysicalObjects = new kpuFixedArray<kpuPhysicalObject*>(aFinalMap.Count());

	//add all instances to this model
	for(int i = 0; i < aFinalMap.Count(); i++)
	{		
		//make new instance
		kpgGeometryInstance* pInst = new kpgGeometryInstance(m_aGeometries[aFinalMap[i].iPiece]);

		pInst->SetPermYRotation( aFinalMap[i].iRotations * -1.570796f );
		kpuVector vPos(aFinalMap[i].fX - fWidth / 2 + aFinalMap[i].vDimensions.GetX() / 2, 0.0f , aFinalMap[i].fY  - iHeigth / 2+ aFinalMap[i].vDimensions.GetZ() / 2, 1.0f);
		pInst->SetPosition(vPos.GetX(), vPos.GetY(), vPos.GetZ());
		
		m_aInstances.Add(pInst);

		vPos += ( aFinalMap[i].vDimensions * -0.5 );
		vPos.SetY(0);
	
		kpgModel* pModel = new kpgModel();
		pModel->SetGeometryInstance(pInst, pInst->GetMatrix());
		//make a new physical object and set its collision mesh
		kpuPhysicalObject* obj = new kpuPhysicalObject(vPos, vPos + aFinalMap[i].vDimensions, pModel);
		
		obj->CalculateBoundingVolumes(aCollisionMeshes[aFinalMap[i].iPiece]);		
		obj->SetFlag(WALL);

		m_paPhysicalObjects->Add(obj);	
	}

	//delete collision meshes
	for(int i = 0; i < aCollisionMeshes.GetNumElements(); i++)
	{
		if ( aCollisionMeshes[i] )
			delete aCollisionMeshes[i];
	}

	return true;
}

bool TerrainModel::SpacesMatch(float fDoorS1, float fDoorE1, float fDoorS2, float fDoorE2, float fLength)
{
	if( ( fDoorS1 >= 0 && fDoorS1 < fLength ) && ( fDoorS1 < fDoorS2 || fDoorS1 > fDoorE2 ) )
		return false;

	if( ( fDoorE1 >= 0 && fDoorE1 < fLength ) && ( fDoorE1 < fDoorS2 || fDoorE1 > fDoorE2 ) )
		return false;

	return true;
}
