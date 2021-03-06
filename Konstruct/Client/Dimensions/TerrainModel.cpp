#include "StdAfx.h"
#include "Common/Graphics/kpgGeometry.h"
#include "Common/Graphics/kpgGeometryInstance.h"
#include "Common/Utility/kpuFixedArray.h"
#include "Common/Utility/kpuArrayList.h"
#include "Common/Utility/kpuLinkedList.h"
#include "TerrainModel.h"
#include "Common/Utility/kpuXmlParser.h"
#include "GameObject.h"

static const u32 s_uHash_Door =		0x7c844039;
static const u32 s_uHash_Wall =		0x7c8e6f75;
static const u32 s_uHash_Data =		0x7c84053f;
static const u32 s_uHash_Top =		0xb8826f8;
static const u32 s_uHash_Left =		0x7c887790;
static const u32 s_uHash_Right =	0xe063143;
static const u32 s_uHash_Bottom =	0xa9edb65a;
static const u32 s_uHash_true =		0x7c9e9fe5;



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

bool TerrainModel::LoadTerrain(const char* pszFile, int iWidth, int iHeigth)
{
	kpuFixedArray<TerrainData> aTerrainData;	
	kpuXmlParser* pParser = new kpuXmlParser();
	bool bRet = false;

	//Load all peices
	if( pParser->LoadFile(pszFile) )
	{
		while( pParser->HasElement() )
		{
			int iCount = pParser->GetAttributeAsInt("Count");
			
			aTerrainData.SetSize(iCount);

			int iPiece = 0;

			pParser->FirstChildElement();
			while( pParser->HasElement() )
			{
				if( pParser->GetValueAsInt() == s_uHash_Data )
				{					
					aTerrainData[iPiece].szModelFile = (char*)pParser->GetAttribute("File");
					aTerrainData[iPiece].szCollisionMeshFile = (char*)pParser->GetAttribute("Collision");

					if( strlen(aTerrainData[iPiece].szCollisionMeshFile) < 1 )
						aTerrainData[iPiece].szCollisionMeshFile = 0;

					aTerrainData[iPiece].iWidth = pParser->GetAttributeAsInt("Width");
					aTerrainData[iPiece].iHeight = pParser->GetAttributeAsInt("Height");
					aTerrainData[iPiece].iLength = pParser->GetAttributeAsInt("Lenght");					

					aTerrainData[iPiece].bStatic = pParser->GetAttributeAsBool("Static");

					pParser->FirstChildElement();
					while( pParser->HasElement() )
					{						
						switch(pParser->GetValueAsInt())
						{
							case s_uHash_Door:
								{
									int iSide = pParser->GetAttributeAsInt("Side");									
									iSide *= 2;		
									char* szData = (char*)pParser->GetChildValue();
				
									aTerrainData[iPiece].aDoorLocations[iSide++] = atoi(&szData[0]);

									while(*szData && *szData != ' ') szData++;
									szData++;

									aTerrainData[iPiece].aDoorLocations[iSide] = atoi(&szData[0]);					
									break;
								}
						}	
						pParser->NextSiblingElement();
					}
					pParser->Parent();

					iPiece++;
				}
				pParser->NextSiblingElement();
			}
			pParser->Parent();
			pParser->NextSiblingElement();
		}
		pParser->Parent();

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

			int iRotation = 0;

			if( !module.bStatic )
			{
				iRotation = rand() % 4;		

				if( iRotation & 1 )
				{
					//flip dimensions
					int xtemp = module.iWidth;
					module.iWidth = module.iLength;
					module.iLength = xtemp;
				}
			}

			kpuLinkedList* pIter = spacesAvailable.Next();

			while(pIter && pIter->GetPointer() )
			{
				//see if object fits in space
				Space* space = (Space*)pIter->GetPointer();

				if( space->iHeight >= module.iLength && space->iWidth >= module.iWidth )
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
					else if( module.iX + module.iWidth == iWidth )
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
					else if( module.iY + module.iLength == iHeigth )
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
						if( iDX > 0 && iDY * iDY <= pTest->iLength * pTest->iLength && iDX * iDX <= pTest->iWidth * pTest->iWidth )
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
							iDoorLen = pTest->iLength;					

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
						else if( iDX < 0 && iDY * iDY <= pTest->iLength * pTest->iLength && iDX * iDX <= pTest->iWidth * pTest->iWidth)
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
							iDoorLen = pTest->iLength;			

							if( iDoorS1 > 0 && iDoorS2 > -1 )
							{
								//- delta Y since it is negative and vertical
								/*iDoorS1 = ( iDoorLen - 1 ) - iDoorS1 - iDY;
								iDoorE1 = ( iDoorLen - 1 ) - iDoorE1 - iDY;*/

								//Conver the peice being added door point into the test peice's wall space
								iDoorE1 = ( module.iLength - 1 - iDoorE1 ) - iDY;
								iDoorS1 = ( module.iLength - 1 - iDoorS1 ) - iDY;

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
						else if( iDY > 0 && iDY * iDY <= pTest->iLength * pTest->iLength && iDX * iDX <= pTest->iWidth * pTest->iWidth )
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
							iDoorLen = pTest->iWidth;					

							if( iDoorS1 > 0 && iDoorS2 > -1 )
							{
								//- delta X since it is negative and horizontal
								/*iDoorS1 = ( iDoorLen - 1 ) - iDoorS1 - iDX;
								iDoorE1 = ( iDoorLen - 1 ) - iDoorE1 - iDX;*/

								//Conver the peice being added door point into the test peice's wall space
								iDoorE1 = ( module.iWidth - 1 - iDoorE1 ) - iDX;
								iDoorS1 = ( module.iWidth - 1 - iDoorS1 ) - iDX;

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
						else if( iDY < 0 && iDY * iDY <= pTest->iLength * pTest->iLength && iDX * iDX <= pTest->iWidth * pTest->iWidth )
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
							iDoorLen = pTest->iWidth;
							
							if( iDoorS1 > 0 && iDoorS2 > -1 )
							{
								//+ delta X since it is positive and Horzontal
								/*iDoorS1 = ( iDoorLen - 1 ) - iDoorS1 + iDX;
								iDoorE1 = ( iDoorLen - 1 ) - iDoorE1 + iDX;*/

								//Conver the peice being added door point into the test peice's wall space
								iDoorE1 = ( pTest->iWidth - 1 - iDoorE1 ) + iDX;
								iDoorS1 = ( pTest->iWidth - 1 - iDoorS1 ) + iDX;

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

						Space* space1 = new Space( space->iX, space->iY + module.iLength, module.iWidth, iHeigth - module.iY - module.iLength );
						
						space->iWidth = space->iWidth - module.iWidth;
						space->iX = module.iX + module.iWidth;

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
			kpuVector vPos(aFinalMap[i].iX - (iWidth * 0.5f) + ( aFinalMap[i].iWidth * 0.5f), 0.0f , aFinalMap[i].iY  - ( iHeigth * 0.5f) + (aFinalMap[i].iLength * 0.5f), 1.0f);
			pInst->SetPosition(vPos.GetX(), vPos.GetY(), vPos.GetZ());
			
			m_aInstances.Add(pInst);

			vPos.SetX(aFinalMap[i].iWidth * -0.5f );
			vPos.SetZ(aFinalMap[i].iLength * -0.5f);
			vPos.SetY(0);
		
			kpgModel* pModel = new kpgModel();
			pModel->SetGeometryInstance(pInst, pInst->GetMatrix());
			//make a new physical object and set its collision mesh
			kpuPhysicalObject* obj = new kpuPhysicalObject(vPos, kpuVector(vPos.GetX() + aFinalMap[i].iWidth, vPos.GetY() + aFinalMap[i].iHeight, vPos.GetZ() + aFinalMap[i].iLength, 0.0f), pModel);
			
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

		bRet = true;
	}

	delete pParser;
	return bRet;
}

bool TerrainModel::SpacesMatch(int iDoorS1, int iDoorE1, int iDoorS2, int iDoorE2, int fLength)
{
	if( ( iDoorS1 >= 0 && iDoorS1 < fLength ) && ( iDoorS1 < iDoorS2 || iDoorS1 > iDoorE2 ) )
		return false;

	if( ( iDoorE1 >= 0 && iDoorE1 < fLength ) && ( iDoorE1 < iDoorS2 || iDoorE1 > iDoorE2 ) )
		return false;

	return true;
}
