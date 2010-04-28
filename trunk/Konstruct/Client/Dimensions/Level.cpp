#include "StdAfx.h"
#include "Level.h"
#include "External/tinyxml/tinyxml.h"
#include "Common/Utility/kpuFileManager.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Graphics/kpgRenderer.h"
#include "Common/Graphics/kpgTerrainModel.h"
#include "Grid.h"
#include "Enemy.h"
#include <time.h>
#include "LoadStructures.h"


static const u32 s_uHash_Name =		0x7c898026;
static const u32 s_uHash_Modules =	0x354414de;
static const u32 s_uHash_Enemies =	0xcb2aaeeb;
static const u32 s_uHash_Mesh =		0x7c890592;
static const u32 s_uHash_Data =		0x7c84053f;
static const u32 s_uHash_Count =	0x0;

Level::Level(void)
{
	m_pTerrain = 0;
	/*m_paModels = 0;*/
	m_paEnemyModels = 0;
	m_pLevelGrid = 0;

	//Seed random
	srand(time(NULL));
}

Level::~Level(void)
{
	/*if( m_paModels )
	{
		for( int i = 0; i < m_paModels->Count(); i++ )
		{
			TerrainModule* pModel = (*m_paModels)[i];
			if( pModel )
				delete pModel;
		}
		delete m_paModels;
	}*/

	if( m_pTerrain )
		delete m_pTerrain;


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

				// Read Elements
                for( TiXmlElement* pElement = pChild->FirstChildElement(); pElement != 0; pElement = pElement->NextSiblingElement() )
                {
                        u32 uHash = StringHash(pElement->Value());
                        if( uHash == s_uHash_Modules )
                        {

							if(	m_pTerrain )
								delete m_pTerrain;

                            m_pTerrain = new kpgTerrainModel();

							if ( !m_pTerrain->LoadTerrain(pElement->FirstChild()->Value(), iWidth, iHeight) )
							{
								bRet = false;
								break;
							}

                            bRet = true;

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


void Level::GenerateEnemies(kpuArrayList<Enemy*> *pEnemies)
{
	
	//Max number to spawn is 4 for now
	kpuVector vGridDim = m_pLevelGrid->GetDimensions() * 0.5;

	int iDistBetweenSpawns = 8;
	int iMaxSpawns = 5;

	int iX = ( vGridDim.GetX() * -1 ) + 1;
	int iY = ( vGridDim.GetZ() * -1 ) + 1;

	while( iX < vGridDim.GetX() && iY < vGridDim.GetZ() )
	{
		int iSpawns =  rand() % iMaxSpawns;
		int iEnemyType = rand() % g_paEnemyTypes->GetNumElements();

		for(int i = 0; i < iSpawns; i++)
		{
			//tile location of enemy
			kpuVector vPos(iX + rand() % iDistBetweenSpawns , 0.0f, iY + rand() % iDistBetweenSpawns, 0.0f);

			Enemy* pEnemy = new Enemy(*(*g_paEnemyTypes)[iEnemyType]);
			pEnemy->SetLocation(vPos);

			//add enemy to the grid
			while( !m_pLevelGrid->AddActor(pEnemy) )
			{
				//move the enemy around till he finds a spot
				vPos.SetX(iX + rand() % iDistBetweenSpawns);
				vPos.SetZ(iY + rand() % iDistBetweenSpawns);
				pEnemy->SetLocation(vPos);
			}

			pEnemies->Add(pEnemy);
		}

		//move to next spawn point
		iX += iDistBetweenSpawns;	

		if(iX  >= vGridDim.GetX() )
		{
			iY += iDistBetweenSpawns;
			iX = ( vGridDim.GetX() * -1 ) + 1;
		}
	}

	//for(int i = 0; i < 3; i++)
	//{
	//	int iType = rand() % iSize;

	//	//Load model
	//	kpgModel* pModel = new kpgModel();
	//	pModel->Load((*pArray)[iType].szModel);

	//	Enemy* pEnemy = new Enemy((*pArray)[iType], pModel);

	//	//get random location
	//	kpuVector vPos((vGridDim.GetX() / 2 * -1) + rand() % (int)vGridDim.GetX(), 0.0,(vGridDim.GetZ() / 2 * -1) + rand() % (int)vGridDim.GetZ(), 0);

	//	pEnemy->SetMoveTarget(m_pLevelGrid->GetTileAtLocation(vPos));

	//	//Set grid
	//	m_pLevelGrid->AddActor(pEnemy);

	//	pEnemies->Add(pEnemy);
	//}

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
	/*if( m_paModels )
	{
		for( int i = 0; i < m_paModels->Count(); i++ )
		{
			(*m_paModels)[i]->Draw(pRenderer);
		}
	}*/

	m_pTerrain->Draw(pRenderer);
}


