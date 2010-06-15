#pragma once

#include "Common/Utility/kpuVector.h"
#include "Common/Utility/kpuLinkedList.h"
#include "GameObject.h"
#include "DamageTypes.h"

class Item;
class Grid;
class PersistentSkill;

#define MAX_PATH_NODES	20

class Actor:public GameObject
{
public:
	Actor(void);
	virtual ~Actor(void);
	
	virtual bool Update(float fGameTime) { return true; }
	virtual void Draw(kpgRenderer* pRenderer);	

	virtual void SetMoveTarget(int iTile)	{ m_iDestinationTile = iTile; m_iCurrentPathNode = -1; }
	void	SetNextMove(int iTile);
	virtual void CancelMovement() { m_iCurrentPathNode = -1; m_iDestinationTile = -1; }
	bool	AtDestination()		{ return m_iDestinationTile < 0; }

	bool BuildPathToDestination();

	virtual float GetExpGiven() { return m_fExpGiven; }

	typedef struct _Reward
	{
		Item**			m_pltReward; //The reward(s) given to the player from this character
		int				m_iNumOfRewards; //The number of rewards given to the player from this character

	}Reward;

	virtual Reward GetReward() { return m_RewardGiven; }

	bool IsInRange(Actor* pTarget, int iRange);
	bool InLineOfSight(Actor* pTarget, int iRange); //Checks if the target is in this actor's line of sight
	
	bool TakeDamage(float fDamage, DamageType eDmgType);
	void DamageBuff(float fAmount) { m_fDamageBuff += fAmount; }
	void DamageReduction(float fAmount) { m_fDamageReduction += fAmount; }
	void Heal(float fAmount);
	void HealMental(float fAmount);
	void RemovePersistentSkill(PersistentSkill* pSkill);
	void AddPersistentSkill(PersistentSkill* pSkill);

	virtual bool UseDefaultAttack(Actor* pTarget, Grid* pGrid);
	bool IsAlive() { return m_fCurrentHealth > 0; }

	void Move(kpuVector vVel);
	void Move(float fDeltaTime, kpuVector vDir);

	void AreaEffect(kpuVector vCenter, float fRadius, void* pEffect, void* pSource);

#pragma region StatAccessor/Mutators

	virtual int GetLevel() { return m_iLevel; }
	virtual float GetSpeed() { return m_fBaseSpeed; }
	kpuVector GetHeading()	{ return m_vHeading; }
	Actor* GetTarget() { return m_pTarget; }
	float GetCurrentHealth() { return m_fCurrentHealth; }
	float GetMaxHealth() { return m_fMaxHealth; }
	float GetMaxMental() { return m_fMaxMental; }
	int GetStr() { return m_iStr; }
	int GetAgi() { return m_iAgi; }
	int GetInt() { return m_iInt; }
	int GetConst() { return m_iConst; }
	int GetDestinationTile() { return m_iDestinationTile; }

	void SetHeading(kpuVector& vHeading);
	void SetTarget(Actor* pTarget) { m_pTarget = pTarget; }
	void SetSpeed(float fSpeed) { m_fBaseSpeed = fSpeed; }
	void SetMaxHealth(float fMaxHealth) { m_fMaxHealth = fMaxHealth; }
	void SetMaxMental(float fMaxMental) { m_fMaxMental = fMaxMental; }
	void SetStr(int iStr) { m_iStr = iStr; }
	void SetAgi(int iAgi) { m_iAgi = iAgi; }
	virtual void SetInt(int iInt) { m_iInt = iInt; }
	virtual void SetConst(int iConst) { m_iConst = iConst; }

#pragma endregion

#pragma region ResisitAccessor/Mutators

	int GetResist(DamageType eType);
	void SetResist(int iAmount, DamageType eType);	

#pragma endregion

protected:
	
	void UpdateMovement(float fDeltaTime);
	

	int				m_iDestinationTile;
	int				m_iCurrentPathNode;
	int				m_aPathNodes[MAX_PATH_NODES];
	
	//Things that need updated
	kpuLinkedList*		m_paPersistentSkills;


	//Movement varibles	
	float			m_fBaseSpeed;			// Movement speed in tiles per second, which is also units per second. 1 tile = 1 unit	
	float			m_fRotation;
	kpuVector		m_vVelocity;
	kpuVector		m_vHeading;	

	//Stats
	float			m_fDamageReduction;
	float			m_fDamageBuff;
	int				m_iLevel;
	int				m_iStr;				//Strength determines how much force player can exert
	int				m_iAgi;				//Agility determines quickly the character can move and react physically
	int				m_iInt;				//Intellegence determines mental pool
	int				m_iConst;			//Constitution determines health pool and resist

	float			m_fMaxHealth;		//The maximum health the player can have atm
	float			m_fCurrentHealth;	//The players current health
	float			m_fMaxMental;		
	float			m_fCurrentMental;

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

#define DEFAULT_MELEE_RANGE 1

