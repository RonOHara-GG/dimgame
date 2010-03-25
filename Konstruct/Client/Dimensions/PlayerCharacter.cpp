#include "StdAfx.h"
#include "PlayerCharacter.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Graphics/kpgGeometryInstance.h"
#include "Skill.h"

PlayerCharacter::PlayerCharacter(void):Actor()
{
	m_pModel = new kpgModel();
	if( !m_pModel->Load("Assets\\Player\\PlayerCard.dae") )
	{
		delete m_pModel;
		m_pModel = 0;
	}	

	m_iLevelBraw = 0;
	m_fCurrExpBraw = 0.0f;
	m_fNeedExpBraw = 0.0f;	

	m_iLevelSword = 0;
	m_fCurrExpSword = 0.0f;
	m_fNeedExpSword = 0.0f;	

	m_iLevelArch = 0;
	m_fCurrExpArch = 0.0f;
	m_fNeedExpArch = 0.0f;	

	m_iLevelMark = 0;
	m_fCurrExpMark = 0.0f;
	m_fNeedExpMark = 0.0f;	

	m_iLevelRock = 0;
	m_fCurrExpRock = 0.0f;
	m_fNeedExpRock = 0.0f;	

	m_iLevelMedi = 0;
	m_fCurrExpMedi = 0.0f;
	m_fNeedExpMedi = 0.0f;	

	m_iLevelPrie = 0;
	m_fCurrExpPrie = 0.0f;
	m_fNeedExpPrie = 0.0f;	

	m_iLevelOccu = 0;
	m_fCurrExpOccu = 0.0f;
	m_fNeedExpOccu = 0.0f;	
}

PlayerCharacter::~PlayerCharacter(void)
{
	if(m_pModel)
		delete m_pModel;
}



void PlayerCharacter::GainExp(int iExp)
{
	if(m_iLevelArch > 0)
	{
		m_fCurrExpArch += iExp * m_fExpSplitArch;

		if(m_fCurrExpArch >= m_fNeedExpArch)
		{
			m_fCurrExpArch -= m_fNeedExpArch;
			LevelUp(eCL_Archer);
			
		}
	}

	if(m_iLevelBraw> 0)
	{
		m_fCurrExpBraw += iExp * m_fExpSplitBraw;

		if(m_fCurrExpBraw >= m_fNeedExpBraw)
		{
			m_fCurrExpBraw -= m_fNeedExpBraw;

			LevelUp(eCL_Brawler);
		}
	}

	if(m_iLevelMark > 0)
	{
		m_fCurrExpMark += iExp * m_fExpSplitMark ;

		if(m_fCurrExpMark  >= m_fNeedExpMark )
		{
			m_fCurrExpMark  -= m_fNeedExpMark ;

			LevelUp(eCL_Marksman);
		}
	}

	if(m_iLevelMedi > 0)
	{
		m_fCurrExpMedi += iExp * m_fExpSplitMedi;

		if(m_fCurrExpMedi >= m_fNeedExpMedi)
		{
			m_fCurrExpMedi -= m_fNeedExpMedi;

			LevelUp(eCL_Medic);
		}
	}

	if(m_iLevelSword > 0)
	{
		m_fCurrExpSword += iExp * m_fExpSplitSword;

		if(m_fCurrExpSword >= m_fNeedExpSword)
		{
			m_fCurrExpSword -= m_fNeedExpSword;

			LevelUp(eCL_Swordsman);
		}
	}

	if(m_iLevelPrie> 0)
	{
		m_fCurrExpPrie += iExp * m_fExpSplitPrie;

		if(m_fCurrExpPrie >= m_fNeedExpPrie)
		{
			m_fCurrExpPrie -= m_fNeedExpPrie;

			LevelUp(eCL_Priest);
		}
	}

	if(m_iLevelRock> 0)
	{
		m_fCurrExpRock += iExp * m_fExpSplitRock;

		if(m_fCurrExpRock >= m_fNeedExpRock)
		{
			m_fCurrExpRock -= m_fNeedExpRock;

			LevelUp(eCL_Rocketeer);
		}
	}

	if(m_iLevelOccu > 0)
	{
		m_fCurrExpOccu += iExp * m_fExpSplitOccu;

		if(m_fCurrExpOccu >= m_fNeedExpOccu)
		{
			m_fCurrExpOccu -= m_fNeedExpOccu;

			LevelUp(eCL_Occultist);
		}
	}




}

void PlayerCharacter::LevelUp(PlayerCharacter::PlayerClasses eClass)
{
	m_iAttribPoints += ATTRIBUTE_POINTS_PER_LEVEL;

	switch(eClass)
	{
		case eCL_Brawler:
			{
				m_iLevelBraw++;
				m_iPointsBraw += SKILL_POINTS_MULTIPLE * m_iLevelBraw;

				break;
			}
		case eCL_Archer:
			{
				m_iLevelArch++;
				m_iPointsArch += SKILL_POINTS_MULTIPLE * m_iLevelArch;

				break;
			}
		case eCL_Marksman:
			{
				m_iLevelMark++;
				m_iPointsMark += SKILL_POINTS_MULTIPLE * m_iLevelMark;

				break;
			}
		case eCL_Medic:
			{
				m_iLevelMedi++;
				m_iPointsMedi += SKILL_POINTS_MULTIPLE * m_iLevelMedi;
				break;
			}
		case eCL_Occultist:
			{
				m_iLevelOccu++;
				m_iPointsOccu += SKILL_POINTS_MULTIPLE * m_iLevelOccu;

				break;
			}
		case eCL_Rocketeer:
			{
				m_iLevelRock++;
				m_iPointsRock += SKILL_POINTS_MULTIPLE * m_iLevelRock;

				break;
			}
		case eCL_Priest:
			{
				m_iLevelPrie++;
				m_iPointsPrie += SKILL_POINTS_MULTIPLE * m_iLevelPrie;

				break;
			}
		case eCL_Swordsman:
			{
				m_iLevelSword++;
				m_iPointsSword += SKILL_POINTS_MULTIPLE * m_iLevelSword;

				break;
			}
	}


}

void PlayerCharacter::Update(float fGameTime)
{
	if( !m_pModel )
		return;

	// Update player position
	UpdateMovement(fGameTime);
}


void PlayerCharacter::UpdateMovement(float fGameTime)
{
	kpuVector vPlayerToTarget = m_vMoveTarget - GetLocation();

	if( vPlayerToTarget.Dot(vPlayerToTarget) > 1 )
	{
		m_vDirection = kpuVector::Normalize(vPlayerToTarget);		
	}
	else
		m_vMoveTarget = GetLocation();

	m_vVelocity = m_vDirection * m_fSpeed;

	SetLocation(GetLocation() + (m_vVelocity * fGameTime));
	
	m_vDirection = kpuv_Zero;
}



void PlayerCharacter::Draw(kpgRenderer* pRenderer)
{
	if( m_pModel )
		m_pModel->Draw(pRenderer);
}
