#pragma once

#include "Common/Utility/kpuVector.h"
#include "GameObject.h"
#include "DamageTypes.h"

class kpgModel;
class kpgRenderer;
class Item;
class Grid;

#define MAX_PATH_NODES	20

class Actor:public GameObject
{
public:
	Actor(void);
	virtual ~Actor(void);
	
	virtual void Update(float fGameTime) {};
	virtual void Draw(kpgRenderer* pRenderer);

	void SetMoveTarget(int iTile)	{ m_iDestinationTile = iTile; m_iCurrentPathNode = -1; }

	virtual kpuVector GetLocation();
	virtual void SetLocation(const kpuVector& vNewLoc);

	virtual float GetExpGiven() { return m_fExpGiven; }

	typedef struct _Reward
	{
		Item**			m_pltReward; //The reward(s) given to the player from this character
		int				m_iNumOfRewards; //The number of rewards given to the player from this character

	}Reward;

	virtual Reward GetReward() { return m_RewardGiven; }

	bool IsInRange(Actor* pTarget, int iRange, Grid* pGrid);
	bool Attackable() { return m_bAttackable; }
	void TakeDamage(float fDamage, DamageType eDmgType);

	virtual void UseDefaultAttack(Actor* pTarget, Grid* pGrid);
	bool IsAlive() { return m_iCurrentHealth > 0; }

#pragma region StatAccessor/Mutators

	int GetMaxHealth() { return m_iMaxHealth; }
	int GetMaxMental() { return m_iMaxMental; }
	int GetStr() { return m_iStr; }
	int GetAgi() { return m_iAgi; }
	int GetInt() { return m_iInt; }
	int GetConst() { return m_iConst; }

	void SetMaxHealth(int iMaxHealth) { m_iMaxHealth = iMaxHealth; }
	void SetMaxMental(int iMaxMental) { m_iMaxMental = iMaxMental; }
	void SetStr(int iStr) { m_iStr = iStr; }
	void SetAgi(int iAgi) { m_iAgi = iAgi; }
	void SetInt(int iInt) { m_iInt = iInt; }
	void SetConst(int iConst) { m_iConst = iConst; }

#pragma endregion

#pragma region ResisitAccessor/Mutators

	int GetCrushRes() { return m_iCrushRes; }
	int GetSlashRes() { return m_iSlashRes; }
	int GetPierceRes() { return m_iPierceRes; }
	int GetMentalRes() { return m_iMentalRes; }
	int GetHeatRes() { return m_iHeatRes; }
	int GetColdRes() { return m_iColdRes; }
	int GetElectRes() { return m_iElectRes; }
	int GetWaterRes() { return m_iWaterRes; }
	int GetAcidRes() { return m_iAcidRes; }
	int GetViralRes() { return m_iViralRes; }
	int GetHolyRes() { return m_iHolyRes; }
	int GetDeathRes() { return m_iDeathRes; }	

	void SetCrushRes(int iCrush) { m_iCrushRes = iCrush; }
	void SetSlashRes(int iSlash) { m_iSlashRes = iSlash; }
	void SetPierceRes(int iPierce) { m_iPierceRes = iPierce; }
	void SetMentalRes(int iMental) { m_iMentalRes = iMental; }
	void SetHeatRes(int iHeat) { m_iHeatRes = iHeat; }
	void SetColdRes(int iCold) { m_iColdRes = iCold; }
	void SetElectRes(int iElect) { m_iElectRes = iElect; }
	void SetWaterRes(int iWater) { m_iWaterRes = iWater; }
	void SetAcidRes(int iAcid) { m_iAcidRes = iAcid; }
	void SetViralRes(int iViral) { m_iViralRes = iViral; }
	void SetHolyRes(int iHoly) { m_iHolyRes = iHoly; }
	void SetDeathRes(int iDeath) {m_iDeathRes = iDeath; }	

#pragma endregion

protected:
	
	void UpdateMovement(float fDeltaTime);
	bool BuildPathToDestination();


	kpgModel*		m_pModel;
	
	int				m_iDestinationTile;
	int				m_iCurrentPathNode;
	int				m_aPathNodes[MAX_PATH_NODES];

	bool			m_bAttackable;		//Wether or not this Actor is able to be attacked

	//Movement varibles	
	float			m_fSpeed;			// Movement speed in tiles per second, which is also units per second. 1 tile = 1 unit
	kpuVector		m_vVelocity;
	kpuVector		m_vDirection;

	//Stats
	int				m_iStr;				//Strength determines how much force player can exert
	int				m_iAgi;				//Agility determines quickly the character can move and react physically
	int				m_iInt;				//Intellegence determines mental pool
	int				m_iConst;				//Constitution determines health pool and resist

	int				m_iMaxHealth;		//The maximum health the player can have atm
	int				m_iCurrentHealth;	//The players current health
	int				m_iMaxMental;		
	int				m_iCurrentMental;

	//Resitances
	int				m_iCrushRes;
	int				m_iSlashRes;
	int				m_iPierceRes;
	int				m_iMentalRes;
	int				m_iHeatRes;
	int				m_iColdRes;
	int				m_iElectRes;
	int				m_iWaterRes;
	int				m_iAcidRes;
	int				m_iViralRes;
	int				m_iHolyRes;
	int				m_iDeathRes;

	GameObject*		m_pobjTarget; //The target of the player

	//Reward
	float			m_fExpGiven; //Amount of exp given to the player from this character	

	Reward			m_RewardGiven;
	Actor*			m_pTarget; //The target of the current actor
};


