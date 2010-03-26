#pragma once

#include "Common/Utility/kpuVector.h"
#include "GameObject.h"


class kpgModel;
class kpgRenderer;
class Item;

#define MAX_PATH_NODES	20

class Actor:public GameObject
{
public:
	Actor(void);
	virtual ~Actor(void);
	
	virtual void Update(float fGameTime) {};
	virtual void Draw(kpgRenderer* pRenderer) {};

	void SetMoveTarget(int iTile)	{ m_iDestinationTile = iTile; }

	virtual kpuVector GetLocation();
	virtual void SetLocation(const kpuVector& vNewLoc);

	virtual float GetExpGiven() { return m_fExpGiven; }

	typedef struct _Reward
	{
		Item**			m_pltReward; //The reward(s) given to the player from this character
		int				m_iNumOfRewards; //The number of rewards given to the player from this character

	}Reward;

	virtual Reward GetReward() { return m_RewardGiven; }

protected:
	
	void UpdateMovement(float fDeltaTime);
	bool BuildPathToDestination();


	kpgModel*		m_pModel;
	
	int				m_iDestinationTile;
	int				m_iCurrentPathNode;
	int				m_aPathNodes[MAX_PATH_NODES];

	//Movement varibles	
	float			m_fSpeed;			// Movement speed in tiles per second, which is also units per second. 1 tile = 1 unit
	kpuVector		m_vVelocity;
	kpuVector		m_vDirection;

	//Stats
	int				m_iStr;				//Strength determines how much force player can exert
	int				m_iAgi;				//Agility determines quickly the character can move and react physically
	int				m_iInt;				//Intellegence determines mental pool
	int				m_iCon;				//Constitution determines health pool and resist

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
};


