#include "StdAfx.h"
#include "MerchantNpc.h"
#include "Item.h"
#include "Common/Graphics/kpgUIManager.h"
#include "PlayerCharacter.h"
#include "External/tinyxml/tinyxml.h"
#include "Common/Utility/kpuFileManager.h"

#define MAX_SHOP_INVENTORY 20
static const u32 s_uHash_MerchantWindow = 0xb6c33c53;
static const u32 s_uHash_ItemList = 0x0000;

MerchantNpc::MerchantNpc(kpgModel* pModel, const char* szName, u32 uType, bool bStatic)
{
	m_pModel = pModel;
	m_pszName = _strdup(szName);
	m_bStatic = bStatic;
	m_uMerchantType = uType;
	
	m_paBasicItems = (Item**)malloc(sizeof(Item*) * MAX_SHOP_INVENTORY);
	memset(m_paBasicItems, 0,sizeof(m_paBasicItems) * MAX_SHOP_INVENTORY);

	m_paTheGoodStuff = (Item**)malloc(sizeof(Item*) * MAX_SHOP_INVENTORY);
	memset(m_paTheGoodStuff, 0,sizeof(m_paTheGoodStuff) * MAX_SHOP_INVENTORY);

	m_pInTransaction = 0;

	m_pItemData = (char***)malloc(sizeof(char*) * MAX_SHOP_INVENTORY);

	for(int i = 0; i < MAX_SHOP_INVENTORY; i++)
	{
		m_pItemData[i] = (char**)malloc(sizeof(char*) * 3);
	} 

	//Load item list from here for now
	LoadItems("Assets/TempItemList.xml");
}

MerchantNpc::~MerchantNpc(void)
{
	delete[] m_paBasicItems;
	delete[] m_paTheGoodStuff;
	free(m_pszName);

	for(int i = 0; i < 20; i++)
	{
		free(m_pItemData[i]);
	} 

	free(m_pItemData);
}
bool MerchantNpc::Update(float fGameTime)
{
	//check if there is a player in transaction with merchant.  If so make sure
	//that the player is still in range, if he isn't close any open dialog

	if( m_pInTransaction )
	{
		if( !IsInRange(m_pInTransaction, m_fActionRange) )
		{
			m_pInTransaction = 0;

			//close open dialog/ windows
			g_pGameState->GetUIManager()->CloseUIWindow(s_uHash_MerchantWindow);		
		}
	}

	return true;
}
void MerchantNpc::Interact(PlayerCharacter* pPlayer)
{
	if( IsInRange(pPlayer, m_fActionRange) )
	{
		//open dialog
		for(int i = 0; i < MAX_SHOP_INVENTORY; i++)
		{
			if( m_paBasicItems[i] )
			{
				m_pItemData[i][0] = m_paBasicItems[i]->GetIcon();				
				m_pItemData[i][1] =	m_paBasicItems[i]->GetDescription();
				m_pItemData[i][2] = m_paBasicItems[i]->GetCostDisplay();				
			}
			else
			{
				m_pItemData[i][0] = 0;
				m_pItemData[i][1] = 0;
				m_pItemData[i][2] = 0;			
				break;
			}
		}
		

		//for now open basic item window
		kpgUIManager* pUIManager = g_pGameState->GetUIManager();		
		pUIManager->SetDataSource(s_uHash_MerchantWindow, (const char*)m_pItemData);
		pUIManager->OpenUIWindow(s_uHash_MerchantWindow);		

		m_pInTransaction = pPlayer;

		/*for(int x = 0; x < 20; x++)
		{
			for(int y = 0; y < 3; y++)
			{
				delete pItemData[x][y];							
			}

			free(pItemData[x]);
		}

		free(pItemData);*/

	}

}

void MerchantNpc::FillInventory()
{
	switch( m_uMerchantType )
	{
	case MT_EQUIPPABLES:
		{
			//generate some random weapons and armor both basic and premium
			break;
		}

	}
}

void MerchantNpc::LoadItems(const char* szFile)
{
	TiXmlDocument doc;
	char szFileName[2048];
	kpuFileManager::GetFullFilePath(szFile, szFileName, sizeof(szFileName));

	if( doc.LoadFile(szFileName) )
	{
		TiXmlElement* pStart = doc.FirstChildElement();
		
		int i = 0;
		for(TiXmlElement* pElement = pStart->FirstChildElement(); pElement != 0 && i < MAX_SHOP_INVENTORY; pElement = pElement->NextSiblingElement() )
		{
			char* szSaleDisplay = (char*)pElement->Attribute("SaleDisplay");
			if( szSaleDisplay )
			{
				//seperate into 3 components icon, description, cost
				char* szIcon = szSaleDisplay;

				char* szDescription = strstr(szIcon, "|");
				*szDescription = 0;
				szDescription++;

				char* szCost = strstr(szDescription, "|");
				*szCost = 0;
				szCost++;

				Item* item = new Item();
				item->SetSaleDisplay(szIcon, szDescription, szCost);

				m_paBasicItems[i] = item;
			}


			i++;
		}

		m_paBasicItems[i] = 0;
	}

}