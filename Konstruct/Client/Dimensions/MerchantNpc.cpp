#include "StdAfx.h"
#include "MerchantNpc.h"
#include "Item.h"
#include "Common/Graphics/kpgUIManager.h"
#include "PlayerCharacter.h"

#define MAX_SHOP_INVENTORY 20
static const u32 s_uHash_MerchantWindow = 0xb6c33c53;
static const u32 s_uHash_ItemList = 0x0000;

MerchantNpc::MerchantNpc(kpgModel* pModel, const char* szName, u32 uType, bool bStatic)
{
	m_pModel = pModel;
	m_pszName = _strdup(szName);
	m_bStatic = bStatic;
	m_uMerchantType = uType;
	
	m_paBasicItems = new Item[MAX_SHOP_INVENTORY];
	memset(m_paBasicItems, 0,sizeof(m_paBasicItems) * MAX_SHOP_INVENTORY);

	m_paTheGoodStuff = new Item[MAX_SHOP_INVENTORY];
	memset(m_paTheGoodStuff, 0,sizeof(m_paTheGoodStuff) * MAX_SHOP_INVENTORY);

	m_pInTransaction = 0;
}

MerchantNpc::~MerchantNpc(void)
{
	delete[] m_paBasicItems;
	delete[] m_paTheGoodStuff;
	free(m_pszName);
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
		const char*** pItemData = (const char***)malloc(sizeof(char*) * 20);

		for(int i = 0; i < 20; i++)
		{
			pItemData[i] = (const char**)malloc(sizeof(char*) * 3);
		} 

		for(int x = 0; x < 20; x++)
		{
			for(int y = 0; y < 3; y++)
			{
				pItemData[x][y] = new char[32];
				sprintf((char*)pItemData[x][y], "Test[%d][%d]", x, y );				
			}
		}

		//for now open basic item window
		kpgUIManager* pUIManager = g_pGameState->GetUIManager();		
		pUIManager->SetDataSource(s_uHash_MerchantWindow, (const char*)pItemData);
		pUIManager->OpenUIWindow(s_uHash_MerchantWindow);		

		m_pInTransaction = pPlayer;

		for(int x = 0; x < 20; x++)
		{
			for(int y = 0; y < 3; y++)
			{
				delete pItemData[x][y];							
			}

			free(pItemData[x]);
		}

		free(pItemData);

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
