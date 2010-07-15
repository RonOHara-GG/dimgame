#pragma once
#include "npc.h"

class Item;

class MerchantNpc :
	public Npc
{
public:
	MerchantNpc(kpgModel* pModel, const char* szName, u32 uType, bool bStatic, char* pszDialog);
	~MerchantNpc(void);
	
	void Interact(PlayerCharacter* pPlayer);
	bool Update(float fGameTime);

	void BuySelectedItem(); //buys selected item from player
	void SellSelectedItem(); // sells selected item to player

	virtual u32 HandleEvent(u32 uEvent);

protected:
	void SetListData();

protected:
	void				LoadItems(const char* szFile);
	void				FillInventory();
	Item**				m_paBasicItems;
	Item**				m_paTheGoodStuff;

	u32					m_uMerchantType;
	u32					m_uSellingWindow; //The name of the window that is used for selling / buying goods


	PlayerCharacter*	m_pInTransaction;  //The player that is currently in a transaction	

	char***				m_pItemData;
	char*				m_pszDialog;

	bool				m_bSelling;
};

#define MT_EQUIPPABLES	0x000000

//input click events
#define CE_OPEN_BUY		0xd19b4847
#define CE_OPEN_SELL	0x50d5e47
#define CE_BUY			0xb87db35
#define CE_SELL			0x0000000



