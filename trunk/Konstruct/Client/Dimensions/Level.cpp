#include "StdAfx.h"
#include "Level.h"
#include "External/tinyxml/tinyxml.h"
#include "Common/Utility/kpuFileManager.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Graphics/kpgRenderer.h"
#include "Common/Graphics/kpgLight.h"
#include "Common/Utility/kpuQuadTree.h"
#include "TerrainModel.h"
#include "Grid.h"
#include "Enemy.h"
#include "LoadStructures.h"
#include "MerchantNpc.h"

static const u32 s_uHash_Name =			0x7c898026;
static const u32 s_uHash_Meshes =		0xc2e0bf8a;
static const u32 s_uHash_Lights =		0xc0cd6650;
static const u32 s_uHash_Terrain =		0x39c3ab7a;
static const u32 s_uHash_Enemies =		0xcb2aaeeb;
static const u32 s_uHash_Mesh =			0x7c890592;
static const u32 s_uHash_Data =			0x7c84053f;
static const u32 s_uHash_Location =		0xedca3cfe;
static const u32 s_uHash_PointLight =	0x09a7ab47;
static const u32 s_uHash_Radius =		0xce39f54d;
static const u32 s_uHash_Color =		0x0cfa2224;
static const u32 s_uHash_UniformScale = 0xb583c2ed;
static const u32 s_uHash_Rotation =		0x9efcecf5;
static const u32 s_uHash_Count =		0xcfa484e;
static const u32 s_uHash_true =			0x7c9e9fe5;
static const u32 s_uHash_Npcs =			0x7c89beb9;
static const u32 s_uHash_Merchcant =	0xfb608e17;

Level::Level(void)
{
	m_pTerrain = 0;
	m_paModels = 0;
	m_paLights = 0;
	m_pLevelGrid = 0;
	m_pQuadTree = 0;
	m_paNpcs = 0;
}

Level::~Level(void)
{
	if( m_paModels )
	{
		for( int i = 0; i < m_paModels->GetNumElements(); i++ )
		{
			kpgModel* pModel = m_paModels->GetElement(i);
			if( pModel )
				delete pModel;
		}
		delete m_paModels;
	}

	if( m_paLights )
	{
		for( int i = 0; i < m_paLights->GetNumElements(); i++ )
		{
			kpgLight* pLight = m_paLights->GetElement(i);
			if( pLight )
				delete pLight;
		}
		delete m_paLights;
	}

	if( m_pTerrain )
		delete m_pTerrain;

	if( m_pQuadTree )
		delete m_pQuadTree;
}

bool Level::Load(const char* pszLevelFile)
{
	kpuArrayList<TerrainModule*>* paModules = new kpuArrayList<TerrainModule*>();

	bool bRet = false;

	char szFileName[2048];
	kpuFileManager::GetFullFilePath(pszLevelFile, szFileName, sizeof(szFileName));
	
	TiXmlDocument doc;

	if( doc.LoadFile(szFileName) )
	{
		for( TiXmlElement* pChild = doc.FirstChildElement(); pChild != 0; pChild = pChild->NextSiblingElement() )
		{
			if( !_strnicmp(pChild->Value(), "Level", 5) )
			{
				// Read the attributes
				strcpy_s(m_szName, sizeof(m_szName), pChild->Attribute("Name"));
				int iWidth = atoi(pChild->Attribute("Width"));
				int iHeight = atoi(pChild->Attribute("Height"));
				assert(!m_pLevelGrid);
				m_pLevelGrid = new Grid(iWidth, iHeight);

				//create quadtree
				if ( m_pQuadTree )
					delete m_pQuadTree;

				m_pQuadTree = new kpuQuadTree(kpuVector( (iWidth + 1) * -0.5f, 0.0f, ( iHeight + 1) * -0.5f, 0.0f), iWidth + 1.0f, iHeight + 1.0f);

				// Read Elements
                for( TiXmlElement* pElement = pChild->FirstChildElement(); pElement != 0; pElement = pElement->NextSiblingElement() )
                {
					u32 uHash = StringHash(pElement->Value());
					if( uHash == s_uHash_Meshes )
					{
						int iMeshCount = atoi(pElement->Attribute("Count"));
						if( m_paModels )
							delete m_paModels;
						m_paModels = new kpuFixedArray<kpgModel*>(iMeshCount);

						for( TiXmlElement* pEChild = pElement->FirstChildElement(); pEChild != 0; pEChild = pEChild->NextSiblingElement() )
						{
							uHash = StringHash(pEChild->Value());
							if( uHash == s_uHash_Mesh )
							{
								kpgModel* pModel = new kpgModel();
								pModel->SetName(StringHash(pEChild->Attribute("Name")));

								bool bLoaded = false;
								for( TiXmlElement* pEChild2 = pEChild->FirstChildElement(); pEChild2 != 0; pEChild2 = pEChild2->NextSiblingElement() )
								{
									uHash = StringHash(pEChild2->Value());
									if( uHash == s_uHash_Data && m_paModels )
									{
										bLoaded = pModel->Load(pEChild2->FirstChild()->Value());
									}
									else if( uHash == s_uHash_Location )
									{
										kpuVector vLoc = ParseCSVVector((char*)pEChild2->FirstChild()->Value());
										pModel->SetPosition(vLoc);
									}
									else if( uHash == s_uHash_UniformScale )
									{
										float fScale = (float)atof(pEChild2->FirstChild()->Value());
										pModel->SetScale(fScale, fScale, fScale);
									}
									else if( uHash == s_uHash_Rotation )
									{
										kpuVector vRot = ParseCSVVector((char*)pEChild2->FirstChild()->Value());
										pModel->Rotate(vRot.GetX() * DEG_TO_RAD, vRot.GetY()* DEG_TO_RAD, vRot.GetZ()* DEG_TO_RAD);
									}
								}

								if( bLoaded )
								{
									m_paModels->Add(pModel);
									bRet = true;
								}
								else
									delete pModel;
							}
						}
					}
					else if( uHash == s_uHash_Lights )
					{
						int iLightCount = atoi(pElement->Attribute("Count"));
						assert(!m_paLights);
						m_paLights = new kpuFixedArray<kpgLight*>(iLightCount);

						for( TiXmlElement* pEChild = pElement->FirstChildElement(); pEChild != 0; pEChild = pEChild->NextSiblingElement() )
						{
							uHash = StringHash(pEChild->Value());
							if( uHash == s_uHash_PointLight )
							{
								kpgLight* pLight = new kpgLight(kpgLight::eLT_Point);

								for( TiXmlElement* pEChild2 = pEChild->FirstChildElement(); pEChild2 != 0; pEChild2 = pEChild2->NextSiblingElement() )
								{
									uHash = StringHash(pEChild2->Value());
									if( uHash == s_uHash_Location )
									{
										kpuVector vLoc = ParseCSVVector((char*)pEChild2->FirstChild()->Value());
										pLight->SetPosition(vLoc);
									}
									else if( uHash == s_uHash_Radius )
									{
										float fRadius = (float)atof(pEChild2->FirstChild()->Value());
										pLight->SetRange(fRadius);
									}
									else if( uHash == s_uHash_Color )
									{
										kpuVector vColor = ParseCSVVector((char*)pEChild2->FirstChild()->Value());
										pLight->SetColor(vColor);
									}
								}

								m_paLights->Add(pLight);
							}
						}
					}
                    else if( uHash == s_uHash_Terrain )
                    {
						if(	m_pTerrain )
							delete m_pTerrain;

                        m_pTerrain = new TerrainModel();

						if ( !m_pTerrain->LoadTerrain(pElement->FirstChild()->Value(), iWidth, iHeight) )
						{
							bRet = false;
							break;
						}

						//add all peices of terrain to quadtree
						kpuFixedArray<kpuPhysicalObject*>* pPieces = m_pTerrain->GetPeices();

						for(int i = 0; i < pPieces->GetNumElements(); i++)
						{
							m_pQuadTree->Add((*pPieces)[i]);
						}

                        bRet = true;
					}
					else if( uHash == s_uHash_Npcs )
					{
						LoadNpcs(pElement);
					}
				}
			}
		}
	}

	return bRet;
}

void Level::LoadNpcs(TiXmlElement* pElement)
{
	if( m_paNpcs )
		delete m_paNpcs;

	m_paNpcs = new kpuFixedArray<Npc*>(atoi(pElement->Attribute("Count")));

	for( TiXmlElement* pChild = pElement->FirstChildElement(); pChild != 0; pChild = pChild->NextSiblingElement() )
	{
		Npc* pNpc = 0;

		const char* szName = pChild->Attribute("Name");
		const char* szModel = pChild->Attribute("Model");
		kpgModel* pModel = 0;

		if( szModel )
		{
			u32 uHash = StringHash(szModel);
			pModel = FindModelByName(uHash);
		}

		szModel = 0;
		szModel = pChild->Attribute("Collision");
		kpgModel* pCollisionModel = 0;

		if( szModel )	
		{	
			pCollisionModel = new kpgModel();
			pCollisionModel->Load(szModel);	
		}

		//Get collision model
		
		bool bStatic = false;
		const char* szStatic = pChild->Attribute("Static");
		if( szStatic )
		{
			u32 uHash = StringHash(szStatic);
			
			bStatic = uHash == s_uHash_true;
		}

		//Get Npc function
		const char* szType = pChild->Attribute("Function");		
		u32 uHashType = StringHash(szType);

		switch(uHashType)
		{
		case s_uHash_Merchcant:
			{
				//Get what kind of items
				szType = pChild->Attribute("MerchantType");		
				uHashType = StringHash(szType);
				
				//Create new merchant
				Npc* pNpc = new MerchantNpc(pModel, szName, uHashType, bStatic);

				if( pCollisionModel )
					pNpc->CalculateBoundingVolumes(pCollisionModel);

				m_paNpcs->Add(pNpc);	

				//Make sure npc is in a tile
				int iTile = m_pLevelGrid->GetTileAtLocation(pNpc->GetLocation());
				pNpc->SetActionRange( (float)atof(pChild->Attribute("ActionRange")) );
				pNpc->SetMoveTarget(iTile);				
				m_pLevelGrid->AddActor(pNpc);
				m_pQuadTree->Add(pNpc);

				break;
			}
		}

		delete pCollisionModel;
	}

	
}

kpgModel* Level::FindModelByName(u32 iNameHash)
{
	for( int i = 0; i < m_paModels->GetNumElements(); i++ )
	{
		kpgModel* pModel = (*m_paModels)[i];
		if( pModel->GetName() == iNameHash )
			return pModel;
	}
	return 0;
}

void Level::GenerateEnemies(kpuArrayList<Actor*> *pActors)
{
	/*Enemy* pEnemy = new Enemy(*(*g_paEnemyTypes)[0]);
	pActors->Add(pEnemy);					
	m_pQuadTree->Add(pEnemy);
	pEnemy->SetLocation(kpuVector(5, 0, 5, 0));
	int iTile = m_pLevelGrid->GetTileAtLocation(kpuVector(5, 0, 5, 0));
	pEnemy->SetNextMove(iTile);*/

	//Commented so I can test skills against 1 enemy

	//Max number to spawn is 4 for now
	int iGridWidth = m_pLevelGrid->GetWidth() / 2;
	int	iGridHeight = m_pLevelGrid->GetHeight() / 2;

	int iDistBetweenSpawns = 8;
	int iMaxSpawns = 5;

	float fX = ( iGridWidth * -1 ) + 0.5f;
	float fY = ( iGridHeight * -1 ) + 0.5f;

	while( fX < iGridWidth && fY < iGridHeight )
	{
		int iSpawns =  rand() % iMaxSpawns;
		int iEnemyType = rand() % g_paEnemyTypes->GetNumElements();

		for(int i = 0; i < iSpawns; i++)
		{
			//tile location of enemy
			kpuVector vPos(fX + rand() % iDistBetweenSpawns , 0.0f, fY + rand() % iDistBetweenSpawns, 0.0f);

			Enemy* pEnemy = new Enemy(*(*g_paEnemyTypes)[iEnemyType]);
			pEnemy->SetLocation(vPos);

			//add enemy to the grid
			while( m_pQuadTree->CheckCollision(pEnemy) || !m_pLevelGrid->AddActor(pEnemy) )
			{
				//move the enemy around till he finds a spot
				vPos.SetX(fX + rand() % iDistBetweenSpawns);
				vPos.SetZ(fY + rand() % iDistBetweenSpawns);

				//int iTile = m_pLevelGrid->GetTileAtLocation(vPos);
				//m_pLevelGrid->GetTileLocation(iTile, vPos);

				pEnemy->SetLocation(vPos);
			}

			pActors->Add(pEnemy);
			
			//add enemy to quad tree			
			m_pQuadTree->Add(pEnemy);
		}

		//move to next spawn point
		fX += iDistBetweenSpawns;	

		if(fX  >= iGridWidth )
		{
			fY += iDistBetweenSpawns;
			fX = ( iGridWidth * -1 ) + 0.5f;
		}
	}
}

void Level::Update(float fDeltaTime)
{
	//update npcs
	for(int i = 0; i < m_paNpcs->GetNumElements(); i++)
	{
		(*m_paNpcs)[i]->Update(fDeltaTime);
	}
}

void Level::Draw(kpgRenderer* pRenderer)
{
	// Bind all the lights
	if( m_paLights )
	{
		for( int i = 0; i < m_paLights->GetNumElements(); i++ )
		{
			kpgLight* pLight = (*m_paLights)[i];
			pRenderer->SetLight(i, pLight);
		}
	}


	if( m_paModels )
	{
		for( int i = 0; i < m_paModels->GetNumElements(); i++ )
		{
			(*m_paModels)[i]->Draw(pRenderer);
		}
	}

	if( m_pTerrain )
		m_pTerrain->Draw(pRenderer);
}


