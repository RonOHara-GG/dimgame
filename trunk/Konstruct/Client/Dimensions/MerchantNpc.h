#pragma once
#include "npc.h"

class Item;

class MerchantNpc :
	public Npc
{
public:
	MerchantNpc(kpgModel* pModel, const char* szName, u32 uType, bool bStatic);
	~MerchantNpc(void);
	
	void Interact(PlayerCharacter* pPlayer);
	bool Update(float fGameTime);

	void BuyItem(PlayerCharacter* pPlayer); //player buys item from merchant
	int SellItem(Item* pItem); // player sells item to merchant and is returned money


protected:
	void				FillInventory();
	Item*				m_paBasicItems;
	Item*				m_paTheGoodStuff;

	u32					m_uMerchantType;
	u32					m_uSellingWindow; //The name of the window that is used for selling / buying goods


	PlayerCharacter*	m_pInTransaction;  //The player that is currently in a transaction	

	char*** m_pItemData;
};

#define MT_EQUIPPABLES	0x000000




