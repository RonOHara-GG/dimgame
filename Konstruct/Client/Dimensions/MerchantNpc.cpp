#include "StdAfx.h"
#include "MerchantNpc.h"
#include "Item.h"
#include "Common/Graphics/kpgUIManager.h"
#include "PlayerCharacter.h"

#define MAX_SHOP_INVENTORY 20

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
		}
	}

	return true;
}
void MerchantNpc::Interact(PlayerCharacter* pPlayer)
{
	if( IsInRange(pPlayer, m_fActionRange) )
	{
		//open dialog

		//for now open basic item window
		

		m_pInTransaction = pPlayer;

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
