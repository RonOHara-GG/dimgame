#include "StdAfx.h"
#include "Level.h"
#include "External/tinyxml/tinyxml.h"
#include "Common/Utility/kpuFileManager.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Graphics/kpgRenderer.h"
#include "Grid.h"
#include "Enemy.h"
#include <time.h>
#include "LoadStructures.h"


static const u32 s_uHash_Name =		0x7c898026;
static const u32 s_uHash_Meshes =	0xc2e0bf8a;
static const u32 s_uHash_Enemies =	0xcb2aaeeb;
static const u32 s_uHash_Mesh =		0x7c890592;
static const u32 s_uHash_Data =		0x7c84053f;
static const u32 s_uHash_Count =	0x0;

Level::Level(void)
{
	m_paModels = 0;
	m_paEnemyModels = 0;
	m_pLevelGrid = 0;

	//Seed random
	srand(time(NULL));
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
}

bool Level::Load(const char* pszLevelFile)
{
	kpuArrayList<kpgModel*>* paModules = new kpuArrayList<kpgModel*>();

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
                        if( uHash == s_uHash_Meshes )
                        {
                                int iMeshCount = atoi(pElement->Attribute("Count"));
                               // if( m_paModels )
                                       // delete m_paModels;

                                //m_paModels = new kpuFixedArray<kpgModel*>(iMeshCount);

								

                                for( TiXmlElement* pEChild = pElement->FirstChildElement(); pEChild != 0; pEChild = pEChild->NextSiblingElement() )
                                {
                                        uHash = StringHash(pEChild->Value());
                                        if( uHash == s_uHash_Mesh )
                                        {
                                                for( TiXmlElement* pEChild2 = pEChild->FirstChildElement(); pEChild2 != 0; pEChild2 = pEChild2->NextSiblingElement() )
                                                {
                                                        uHash = StringHash(pEChild2->Value());
                                                        if( uHash == s_uHash_Data && paModules ) //m_paModels )
                                                        {                                                                               
                                                                kpgModel* pModel = new kpgModel();                                                                              
                                                                if( pModel->Load(pEChild2->FirstChild()->Value()) )
                                                                {
                                                                        //m_paModels->Add(pModel);
																		paModules->Add(pModel);
                                                                        bRet = true;
                                                                }
                                                        }
                                                }
                                        }
                                }


						}
				}
			}
		}
	}



	//attempt to build terrain
	GenerateTerrain(paModules);

	delete paModules;

	return bRet;
}

void Level::LoadEnemyList(kpuArrayList<Enemy*>* paEnemies)
{
	char szFileName[2048];
	TiXmlDocument doc;

	kpuFileManager::GetFullFilePath("/Assets/EnemyList.xml", szFileName, sizeof(szFileName));
	//Load all enemy types and generate them
	if(doc.LoadFile(szFileName))
	{
		for(TiXmlElement* pElement = doc.FirstChildElement(); pElement != 0; pElement = pElement->NextSiblingElement())
		{
			int iCount = atoi(pElement->Attribute("Count"));

			kpuFixedArray<EnemyLoadStructure>* paEnemyTypes = new kpuFixedArray<EnemyLoadStructure>(iCount);

			for(TiXmlElement* pChild = pElement->FirstChildElement(); pChild != 0; pChild = pChild->NextSiblingElement())
			{
				const char* szFilename = pChild->Attribute("File");

				LoadEnemyType(szFilename, paEnemyTypes);
			}

			if(iCount > 0)
			{
				//Make some enemies
				GenerateEnemies(paEnemyTypes, paEnemies, iCount);
			}

		}
	}

}


void Level::GenerateEnemies(kpuFixedArray<EnemyLoadStructure> *pArray, kpuArrayList<Enemy*> *pEnemies, int iSize)
{
	//Max number to spawn is 3 for now
	kpuVector vGridDim = m_pLevelGrid->GetDimensions();

	for(int i = 0; i < 3; i++)
	{
		int iType = rand() % iSize;

		//Load model
		kpgModel* pModel = new kpgModel();
		pModel->Load((*pArray)[iType].szModel);

		Enemy* pEnemy = new Enemy((*pArray)[iType], pModel);

		//get random location
		kpuVector vPos((vGridDim.GetX() / 2 * -1) + rand() % (int)vGridDim.GetX(), 0.0,(vGridDim.GetZ() / 2 * -1) + rand() % (int)vGridDim.GetZ(), 0);

		pEnemy->SetMoveTarget(m_pLevelGrid->GetTileAtLocation(vPos));

		//Set grid
		m_pLevelGrid->AddActor(pEnemy);

		pEnemies->Add(pEnemy);
	}

}

void Level::LoadEnemyType(const char* szFilename, kpuFixedArray<EnemyLoadStructure>* pArray)
{
	FILE* pFile = 0;
	pFile = fopen(szFilename, "rb");

	if(pFile)
	{
		EnemyLoadStructure enemyLoad;

		//Get the enemy name and dae file location
		fread(&enemyLoad.iNameLength, 4, 2, pFile);

		fread(enemyLoad.szName, enemyLoad.iNameLength, 1, pFile);
		fread(enemyLoad.szModel, enemyLoad.iFileLength, 1, pFile);

		enemyLoad.szName[enemyLoad.iNameLength] = 0;
		enemyLoad.szModel[enemyLoad.iFileLength] = 0;

		//read rest of data
		fread((char*)&enemyLoad.iLevel, sizeof(enemyLoad) - enemyLoad.iNameLength - enemyLoad.iFileLength, 1, pFile);


		pArray->Add(enemyLoad);

		fclose(pFile);
	}

}

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
}

void Level::GenerateTerrain(kpuArrayList<kpgModel*>* paModules)
{
	if( m_paModels )
       delete m_paModels;

    m_paModels = new kpuFixedArray<kpgModel*>(paModules->Count());

	//Get the first module to build from
	int iModule = rand() % paModules->Count();
	kpgModel* module = (*paModules)[iModule];

	if ( m_pLevelGrid->AddTerrain(module) )
	{
		//Add to list of final terrain
		m_paModels->Add(module);
		paModules->RemoveAt(iModule);

		//Keep adding until avilable modules are all used
		while ( paModules->Count() > 0 )
		{
			//Get another module
			module = (*paModules)[0];

			//Build off of a random terrain already added
			kpgModel* base = (*m_paModels)[rand() % m_paModels->GetNumElementsUsed() ];

			//Find which end to build from for now
			kpuVector vBuildNorm(0,0,0,0);

			do 
			{
				float fNorm = 0.0f;

				//Find the build norm
				while (fNorm == 0.0f)
				{
					int XorZ = rand() % 2;

					if(XorZ == 0 )
					{
						fNorm = -1 + rand() % 3;
						vBuildNorm.SetX(fNorm);
					}

					if ( XorZ == 1 )
					{
						fNorm = -1 + rand() % 3;
						vBuildNorm.SetZ(fNorm);
					}
				}

				kpuVector vLocation = base->GetPosition();

				//Get location at end where we will build
				vLocation += kpuVector( ( base->GetDimensions().GetX() / 2 ) * vBuildNorm.GetX(), 0, ( base->GetDimensions().GetZ() / 2 ) * vBuildNorm.GetZ(), 0);

				//add in the dimensions the piece we are building
				vLocation += kpuVector( ( module->GetDimensions().GetX() / 2 ) * vBuildNorm.GetX(), 0, ( module->GetDimensions().GetZ() / 2 ) * vBuildNorm.GetZ(), 0);	

				//Set location anywhere along width of base edge
				if ( vBuildNorm.GetX() != 0 )
					vLocation.SetZ( vLocation.GetZ() + (int)( -1 * base->GetDimensions().GetZ() / 2 ) + ( rand() % (int)base->GetDimensions().GetZ() ));
				else
					vLocation.SetX( vLocation.GetX() + (int)( -1 * base->GetDimensions().GetX() / 2 ) + ( rand() % (int)base->GetDimensions().GetX() ));
				

				module->SetPosition(vLocation);

			}while( !m_pLevelGrid->AddTerrain(module) );

			paModules->RemoveAt(0);
			m_paModels->Add(module);

		}
	}







}
