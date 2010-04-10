#include "StdAfx.h"
#include "Level.h"
#include "External/tinyxml/tinyxml.h"
#include "Common/Utility/kpuFileManager.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Graphics/kpgRenderer.h"
#include "Grid.h"
#include "Enemy.h"
#include <time.h>


static const u32 s_uHash_Name =		0x7c898026;
static const u32 s_uHash_Meshes =	0xc2e0bf8a;
static const u32 s_uHash_Enemies =	0xcb2aaeeb;
static const u32 s_uHash_Terrain =  0x39c3ab7a;
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
						for( TiXmlElement* pEChild = pElement->FirstChildElement(); pEChild != 0; pEChild = pEChild->NextSiblingElement() )
						{
							uHash = StringHash(pEChild->Value());
							if( uHash == s_uHash_Mesh )
							{								
								for( TiXmlElement* pEChild2 = pEChild->FirstChildElement(); pEChild2 != 0; pEChild2 = pEChild2->NextSiblingElement() )
								{
									const char* name = pEChild2->Value();

									u32 uHash = StringHash(name);
									int iMeshCount = atoi(pEChild2->Attribute("Count"));
									switch(uHash)
									{
										case s_uHash_Terrain:
											{
												if( m_paModels )
													delete m_paModels;

												m_paModels = new kpuFixedArray<kpgModel*>(iMeshCount);
												
												for( TiXmlElement* pEChild3 = pEChild2->FirstChildElement(); pEChild3 != 0; pEChild3 = pEChild3->NextSiblingElement() )
												{
													uHash = StringHash(pEChild3->Value());
													if( uHash == s_uHash_Data && m_paModels )
													{										
														kpgModel* pModel = new kpgModel();										
														if( pModel->Load(pEChild3->FirstChild()->Value()) )
														{
															m_paModels->Add(pModel);
															bRet = true;
														}
													}
												}

												break;
											}
										case s_uHash_Enemies:
											{
												if( m_paEnemyModels )
													delete m_paEnemyModels;

												m_paEnemyModels = new kpuFixedArray<kpgModel*>(iMeshCount);
												
												for( TiXmlElement* pEChild3 = pEChild2->FirstChildElement(); pEChild3 != 0; pEChild3 = pEChild3->NextSiblingElement() )
												{
													uHash = StringHash(pEChild3->Value());
													if( uHash == s_uHash_Data && m_paEnemyModels )
													{										
														kpgModel* pModel = new kpgModel();										
														if( pModel->Load(pEChild3->FirstChild()->Value()) )
														{
															m_paEnemyModels->Add(pModel);
															bRet = true;
														}
													}
												}

												break;
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

	return bRet;
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

	/*if( m_paEnemyModels )
	{
		for( int i = 0; i < m_paEnemyModels->GetNumElements(); i++ )
		{
			(*m_paEnemyModels)[i]->Draw(pRenderer);
		}
	}*/

}

