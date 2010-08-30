#include "StdAfx.h"
#include "MerchantNpc.h"
#include "Item.h"
#include "Common/Graphics/kpgUIManager.h"
#include "Common/Graphics/kpgUIList.h"
#include "PlayerCharacter.h"
#include "Common/Utility/kpuXmlParser.h"

#define SELL_BACK_RATIO 2
static const u32 s_uHash_MerchantWindow = 0xb6c33c53;
static const u32 s_uHash_MerchantDialogWin = 0x7b27095;
static const u32 s_uHash_Static = 0x00000;
static const u32 s_uHash_Dynamic = 0x0001;

MerchantNpc::MerchantNpc()
{
	m_pModel = 0;
	m_pszName = 0;
	m_pszDialog = 0;
	m_bStatic = true;
	m_uMerchantType = 0;
	
	m_paBasicItems = 0;

	m_pInTransaction = 0;
	

	SetFlag(MERCHANT);
}

MerchantNpc::~MerchantNpc(void)
{
	for(int i = 0; i < m_paBasicItems->GetNumElements(); i++)
	{
		if( (*m_paBasicItems)[i] )
			delete (*m_paBasicItems)[i];
		
		if( m_pItemData[i] )
		{
			free(m_pItemData[i]);
			m_pItemData[i] = 0;
		}
	}

	delete m_paBasicItems;

	free(m_pszName);
	free(m_pszDialog);
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

		m_pInTransaction = pPlayer;

	}

}
void MerchantNpc::SetListData()
{
	int j = 0;
	for(int i = 0; i < m_paBasicItems->GetNumElements(); i++)
	{
		if( (*m_paBasicItems)[i] )
		{
			m_pItemData[j][0] = (*m_paBasicItems)[i]->GetIcon();				
			m_pItemData[j][1] =	(*m_paBasicItems)[i]->GetDescription();
			m_pItemData[j][2] = (*m_paBasicItems)[i]->GetCostDisplay();	
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

}

void MerchantNpc::LoadItems(const char* szFile)
{
	kpuXmlParser* pParser = new kpuXmlParser();
	if( pParser->LoadFile(szFile) )
	{		
		int i = 0;
		pParser->FirstChildElement();
		while( pParser->HasElement() && i < m_paBasicItems->GetNumElements())
		{
			char* szSaleDisplay = (char*)pParser->GetAttribute("SaleDisplay");
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

				m_paBasicItems->Add(item);
			}


			i++;
			pParser->NextSiblingElement();
		}

		pParser->Parent();

		m_paBasicItems[i] = 0;
	}

	delete pParser;

}
void MerchantNpc::SellSelectedItem()
{
	//Get the selected item from the merchant list	
	kpgUIManager* pUIManager = g_pGameState->GetUIManager();		
	kpgUIList* pItemList = (kpgUIList*)pUIManager->GetUIWindow(s_uHash_MerchantWindow);

	int iIndex = pItemList->GetSelectedRow();

	if( iIndex > -1 && iIndex < m_paBasicItems->GetNumElements()&& m_pInTransaction->GetMoney() >= (*m_paBasicItems)[iIndex]->GetCost() )
	{
		//sell item to player
		if( m_pInTransaction->AddItemToInventory((*m_paBasicItems)[iIndex]) )
		{
			m_pInTransaction->AddMoney(-(*m_paBasicItems)[iIndex]->GetCost());

			(*m_paBasicItems)[iIndex] = 0;

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
			for(int i = 0; i < m_paBasicItems->GetNumElements(); i++)
			{
				if( !(*m_paBasicItems)[i] )
				{
					(*m_paBasicItems)[i] = pItem;
					return;
				}
			}
			
			//no room delete item
			delete pItem;			
		}	
		
	}
}

u32 MerchantNpc::HandleEvent(u32 uEvent, u32 uParam)
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

void MerchantNpc::Load(kpuXmlParser* pParser, kpgModel* pModel)
{
	Npc::Load(pParser, pModel);

	//Get dialog
	char* szDialog = (char*)pParser->GetAttribute("Dialog");

	if( szDialog )
		m_pszDialog = _strdup(szDialog);

	//load inventory list
	pParser->FirstChildElement();
	kpuXmlParser* pInvParser = new kpuXmlParser();
	if( pInvParser->LoadFile(pParser->GetChildValue()) )
	{
		int iSize = 0;

		iSize = pInvParser->GetAttributeAsInt("Count");

		if( m_paBasicItems )
			delete m_paBasicItems;

		m_paBasicItems = new kpuFixedArray<Item*>(iSize);

		pInvParser->FirstChildElement();
		while( pInvParser->HasElement() )
		{
			switch((u32)pInvParser->GetValueAsInt())
			{
			case s_uHash_Static:
				break;
			case s_uHash_Dynamic:
				break;
			}

			pInvParser->NextSiblingElement();
		}

		pInvParser->Parent();
	}

	pParser->Parent();
	delete pInvParser;

	m_pItemData = (char***)malloc(m_paBasicItems->GetNumElements() * sizeof(char*));

	for(int i = 0; i < m_paBasicItems->GetNumElements(); i++)
	{
		m_pItemData[i] = (char**)calloc(3, sizeof(char*));
	} 

	LoadItems("Assets/TempItemList.xml");
}