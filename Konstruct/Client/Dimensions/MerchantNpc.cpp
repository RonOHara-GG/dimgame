#include "StdAfx.h"
#include "MerchantNpc.h"
#include "Item.h"
#include "Common/Graphics/kpgUIManager.h"
#include "Common/Graphics/kpgUIList.h"
#include "PlayerCharacter.h"
#include "External/tinyxml/tinyxml.h"
#include "Common/Utility/kpuFileManager.h"

#define MAX_SHOP_INVENTORY 20
#define SELL_BACK_RATIO 2
static const u32 s_uHash_MerchantWindow = 0xb6c33c53;
static const u32 s_uHash_MerchantDialogWin = 0x7b27095;

MerchantNpc::MerchantNpc(kpgModel* pModel, const char* szName, u32 uType, bool bStatic, char* pszDialog)
{
	m_pModel = pModel;
	m_pszName = _strdup(szName);
	m_pszDialog = _strdup(pszDialog);
	m_bStatic = bStatic;
	m_uMerchantType = uType;
	
	m_paBasicItems = (Item**)calloc(MAX_SHOP_INVENTORY, sizeof(Item*));

	m_paTheGoodStuff = (Item**)calloc(MAX_SHOP_INVENTORY, sizeof(Item*));

	m_pInTransaction = 0;

	m_pItemData = (char***)malloc(MAX_SHOP_INVENTORY * sizeof(char*));

	for(int i = 0; i < MAX_SHOP_INVENTORY; i++)
	{
		m_pItemData[i] = (char**)calloc(3, sizeof(char*));
	} 


	//Load item list from here for now
	LoadItems("Assets/TempItemList.xml");

	SetFlag(MERCHANT);
}

MerchantNpc::~MerchantNpc(void)
{
	delete[] m_paBasicItems;
	delete[] m_paTheGoodStuff;
	free(m_pszName);

	for(int i = 0; i < 20; i++)
	{
		if( m_pItemData[i] )
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
		m_pInTransaction->SetTarget(this);

		if( !IsInRange(m_pInTransaction, m_fActionRange) )
		{
			m_pInTransaction = 0;

			//close open dialog/ windows
			g_pGameState->GetUIManager()->CloseUIWindow(s_uHash_MerchantDialogWin);
			g_pGameState->GetUIManager()->CloseUIWindow(s_uHash_MerchantWindow);		
		}
	}

	return true;
}
void MerchantNpc::Interact(PlayerCharacter* pPlayer)
{
	if( IsInRange(pPlayer, m_fActionRange) )
	{
		SetListData();

		//open dialog		
		kpgUIManager* pUIManager = g_pGameState->GetUIManager();
		pUIManager->SetDataSource("Dialog", m_pszDialog);
		pUIManager->OpenUIWindow(s_uHash_MerchantDialogWin, this);	

		//pUIManager->OpenUIWindow(s_uHash_MerchantWindow, this);		



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
void MerchantNpc::SetListData()
{
	int j = 0;
	for(int i = 0; i < MAX_SHOP_INVENTORY; i++)
	{
		if( m_paBasicItems[i] )
		{
			m_pItemData[j][0] = m_paBasicItems[i]->GetIcon();				
			m_pItemData[j][1] =	m_paBasicItems[i]->GetDescription();
			m_pItemData[j][2] = m_paBasicItems[i]->GetCostDisplay();	
			j++;
		}		
			
	}	

	m_pItemData[j][0] = 0;	

	//for now open basic item window
	kpgUIManager* pUIManager = g_pGameState->GetUIManager();		
	pUIManager->SetDataSource("ItemList", (char*)m_pItemData);

	


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

				char* szDescription = strchr(szIcon, ',');
				*szDescription = 0;
				szDescription++;

				char* szCost = strchr(szDescription, ',');
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
void MerchantNpc::SellSelectedItem()
{
	//Get the selected item from the merchant list	
	kpgUIManager* pUIManager = g_pGameState->GetUIManager();		
	kpgUIList* pItemList = (kpgUIList*)pUIManager->GetUIWindow(s_uHash_MerchantWindow);

	int iIndex = pItemList->GetSelectedRow();

	if( iIndex > -1 && iIndex < MAX_SHOP_INVENTORY && m_pInTransaction->GetMoney() >= m_paBasicItems[iIndex]->GetCost() )
	{
		//sell item to player
		if( m_pInTransaction->AddItemToInventory(m_paBasicItems[iIndex]) )
		{
			m_pInTransaction->AddMoney(-m_paBasicItems[iIndex]->GetCost());

			m_paBasicItems[iIndex] = 0;

			//reset list data with updated list
			SetListData();
		}

		//if no room don't add possibly display text to player stating not enough room
	}
}

void MerchantNpc::BuySelectedItem()
{
	kpgUIManager* pUIManager = g_pGameState->GetUIManager();		
	kpgUIList* pItemList = (kpgUIList*)pUIManager->GetUIWindow(s_uHash_MerchantWindow);

	int iIndex = pItemList->GetSelectedRow();

	if( iIndex > -1 && iIndex < INVENTORY_SIZE )
	{
		//get the item from the player
		Item* pItem = m_pInTransaction->RemoveFromInventory(iIndex);

		if( pItem )
		{		
			//take item and pay player
			m_pInTransaction->AddMoney(pItem->GetCost() / SELL_BACK_RATIO);
			
			//if possible add to merchant inventory
			for(int i = 0; i < MAX_SHOP_INVENTORY; i++)
			{
				if( !m_paBasicItems[i] )
				{
					m_paBasicItems[i] = pItem;
					return;
				}
			}
			
			//no room delete item
			delete pItem;			
		}	
		
	}
}

u32 MerchantNpc::HandleEvent(u32 uEvent)
{
	if( uEvent == 0 )
		return 0;

	switch( uEvent )
		{
		case CE_OPEN_SELL:
			{
				//open the window for the player to buy items
				kpgUIManager* pUIManager = g_pGameState->GetUIManager();				
				SetListData();
				pUIManager->OpenUIWindow(s_uHash_MerchantWindow, this);
				g_pGameState->GetUIManager()->CloseUIWindow(s_uHash_MerchantDialogWin);
				m_bSelling = true;
				return 0;
			}
		case CE_OPEN_BUY:
			{
				//open the window for the player to buy items
				kpgUIManager* pUIManager = g_pGameState->GetUIManager();				
				m_pInTransaction->SetInventoryList();
				pUIManager->OpenUIWindow(s_uHash_MerchantWindow, this);
				g_pGameState->GetUIManager()->CloseUIWindow(s_uHash_MerchantDialogWin);
				m_bSelling = false;
				return 0;
			}
		case CE_BUY:			
			//buy the selected item from the player's current target
			if( m_bSelling )
				SellSelectedItem();	
			else
				BuySelectedItem();

			return 0;			
		case CE_SELL:
			BuySelectedItem();
			return 0;
		}

	return IE_NOT_HANDLED;
}