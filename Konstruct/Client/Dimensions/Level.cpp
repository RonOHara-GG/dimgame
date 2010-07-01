#include "StdAfx.h"
#include "Level.h"
#include "External/tinyxml/tinyxml.h"
#include "Common/Utility/kpuFileManager.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Graphics/kpgRenderer.h"
#include "TerrainModel.h"
#include "Common/Utility/kpuQuadTree.h"
#include "Grid.h"
#include "Enemy.h"
#include <time.h>
#include "LoadStructures.h"


static const u32 s_uHash_Name =			0x7c898026;
static const u32 s_uHash_Terrain =		0x39c3ab7a;
static const u32 s_uHash_Enemies =		0xcb2aaeeb;
static const u32 s_uHash_Meshes =		0xc2e0bf8a;
static const u32 s_uHash_Mesh =			0x7c890592;
static const u32 s_uHash_Data =			0x7c84053f;
static const u32 s_uHash_Count =		0xcfa484e;
static const u32 s_uHash_Background =	0xe62bf365;

Level::Level(void)
{
	m_pTerrain = 0;
	m_paModels = 0;
	m_paEnemyModels = 0;
	m_pLevelGrid = 0;
	m_pQuadTree = 0;

	//Seed random
	srand((unsigned int)time(NULL));

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
                        switch( uHash )
                        {
						case s_uHash_Meshes:
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
                                                for( TiXmlElement* pEChild2 = pEChild->FirstChildElement(); pEChild2 != 0; pEChild2 = pEChild2->NextSiblingElement() )
                                                {
                                                        uHash = StringHash(pEChild2->Value());
                                                        if( uHash == s_uHash_Data && m_paModels )
                                                        {                                                                               
                                                                kpgModel* pModel = new kpgModel();                                                                              
                                                                if( pModel->Load(pEChild2->FirstChild()->Value()) )
                                                                {
                                                                        m_paModels->Add(pModel);
                                                                        bRet = true;
                                                                }
                                                        }
                                                }
                                        }
                                }
								break;
							}
						case s_uHash_Terrain:
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


								break;
							}
							
                        }
                }
				
			}
		}
	}
	return bRet;
}

//void Level::LoadEnemyList(kpuArrayList<Enemy*>* paEnemies)
//{
//	char szFileName[2048];
//	TiXmlDocument doc;
//
//	kpuFileManager::GetFullFilePath("/Assets/EnemyData/EnemyMasterList.xml", szFileName, sizeof(szFileName));
//	//Load all enemy types and generate them
//	if(doc.LoadFile(szFileName))
//	{
//		for(TiXmlElement* pElement = doc.FirstChildElement(); pElement != 0; pElement = pElement->NextSiblingElement())
//		{
//			int iCount = atoi(pElement->Attribute("Count"));
//
//			kpuFixedArray<EnemyLoadStructure>* paEnemyTypes = new kpuFixedArray<EnemyLoadStructure>(iCount);
//
//			for(TiXmlElement* pChild = pElement->FirstChildElement(); pChild != 0; pChild = pChild->NextSiblingElement())
//			{
//				const char* szFilename = pChild->FirstChild()->Value();
//
//				LoadEnemyType(szFilename, paEnemyTypes);
//			}
//
//			if(iCount > 0)
//			{
//				//Make some enemies
//				GenerateEnemies(paEnemyTypes, paEnemies, iCount);
//			}
//
//		}
//	}
//
//}


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

//void Level::LoadEnemyType(const char* pszFile, kpuFixedArray<EnemyLoadStructure>* pArray)
//{
//	char szFilename[2048];
//
//	kpuFileManager::GetFullFilePath(pszFile, szFilename, sizeof(szFilename) );
//
//	TiXmlDocument doc;
//
//	if( doc.LoadFile(szFilename) )
//	{
//		TiXmlElement* pElement = doc.FirstChildElement();
//
//		EnemyLoadStructure enemyType;
//		enemyType.pszName = (char*)pElement->Attribute("Name");
//		enemyType.iLevel = atoi(pElement->Attribute("Level"));
//		enemyType.iHealth = atoi(pElement->Attribute("Health"));
//		enemyType.fSpeed = atof(pElement->Attribute("Speed"));
//		enemyType.pModel = new kpgModel();
//		enemyType.pModel->Load(pElement->Attribute("Model"));
//		enemyType.iDamage = atoi(pElement->Attribute("Damage"));
//		enemyType.fAggroRange = atof(pElement->Attribute("Aggro"));
//		enemyType.fAttackRange = atof(pElement->Attribute("AtkRange"));
//		enemyType.fAttackSpeed = atof(pElement->Attribute("AtkSpeed"));
//		enemyType.iDamageType = atof(pElement->Attribute("DamageType"));
//
//		//goto resits
//		TiXmlElement* pResits = pElement->FirstChildElement();
//
//		enemyType.iCrushRes = atoi(pResits->Attribute("Crushing"));
//		enemyType.iPierceRes = atoi(pResits->Attribute("Piercing"));
//		enemyType.iSlashRes = atoi(pResits->Attribute("Slashing"));	
//		enemyType.iMentalRes = atoi(pResits->Attribute("Mental"));
//		enemyType.iHeatRes = atoi(pResits->Attribute("Heat"));
//		enemyType.iColdRes = atoi(pResits->Attribute("Cold"));
//		enemyType.iAcidRes = atoi(pResits->Attribute("Acid"));
//		enemyType.iViralRes = atoi(pResits->Attribute("Viral"));
//		enemyType.iHolyRes = atoi(pResits->Attribute("Holy"));
//		enemyType.iWaterRes = atoi(pResits->Attribute("Water"));
//		enemyType.iDeathRes = atoi(pResits->Attribute("Death"));
//		enemyType.iElectRes = atoi(pResits->Attribute("Electric"));
//
//		pArray->Add(enemyType);
//	}
//    
//	//FILE* pFile = 0;
//	//pFile = fopen(szFilename, "rb");
//
//	//if(pFile)
//	//{
//	//	EnemyenemyTypeure enemyLoad;
//
//	//	//Get the enemy name and dae file location
//	//	fread(&enemyLoad.iNameLength, 4, 2, pFile);
//
//	//	fread(enemyLoad.szName, enemyLoad.iNameLength, 1, pFile);
//	//	fread(enemyLoad.szModel, enemyLoad.iFileLength, 1, pFile);
//
//	//	enemyLoad.szName[enemyLoad.iNameLength] = 0;
//	//	enemyLoad.szModel[enemyLoad.iFileLength] = 0;
//
//	//	//read rest of data
//	//	fread((char*)&enemyLoad.iLevel, sizeof(enemyLoad) - enemyLoad.iNameLength - enemyLoad.iFileLength, 1, pFile);
//
//
//	//	pArray->Add(enemyLoad);
//
//	//	fclose(pFile);
//	//}
//}

void Level::Update()
{
}

void Level::Draw(kpgRenderer* pRenderer)
{
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


