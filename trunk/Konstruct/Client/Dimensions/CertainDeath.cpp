#include "StdAfx.h"
#include "CertainDeath.h"
#include "Weapon.h"
#include "PlayerCharacter.h"
#include "Level.h"
#include "Common/utility/kpuArrayList.h"
#include "Common/utility/kpuQuadTree.h"
#include "Common/utility/kpuCollisionData.h"
#include "Common/utility/kpuBoundingSphere.h"

CertainDeath::CertainDeath(void)
{
}

CertainDeath::~CertainDeath(void)
{
}

bool CertainDeath::Activate(PlayerCharacter *pSkillOwner)
{
	return true;
}

bool CertainDeath::Update(PlayerCharacter *pSkillOwner, float fDeltaTime)
{
	Weapon* pEquipped = pSkillOwner->GetEquippedWeapon();
	int iRankMultiple = Skill::m_iRankMultipleMin + ( rand() % (int)(Skill::m_iRankMultipleMax - Skill::m_iRankMultipleMin) );
	m_fRange = pEquipped->GetRange();
	m_fDamage = pEquipped->GetDamage() + iRankMultiple * Skill::m_iSkillRank;
	m_eDamageType = pEquipped->GetDamageType();
	Skill::m_fSpeed = pEquipped->GetSpeed();
	Skill::m_fRecovery = pEquipped->GetRecovery();

	//see if target is dead
	if( !pSkillOwner->GetTarget()->IsAlive() )
	{
		//find a new target and shoot it
		kpuBoundingSphere sphere(m_fRange, pSkillOwner->GetLocation());
		
		kpuArrayList<kpuCollisionData> aCollisions;

		g_pGameState->GetLevel()->GetQuadTree()->GetPossibleCollisions(sphere, &aCollisions);

		Actor* pClosest = 0;
		float fDist = m_fRange * m_fRange;

		for(int i = 0; i < aCollisions.Count(); i++)
		{
			kpuCollisionData* pNext = &aCollisions[i];

			if( pNext->m_pObject->HasFlag(ATTACKABLE) )
			{
				Actor* pTarget = (Actor*)pNext->m_pObject;
				float fNextDist = kpuVector::DistanceSquared(pSkillOwner->GetLocation(), pTarget->GetLocation() );
				if( fNextDist < fDist )
				{
					if( pTarget->InLineOfSight(pSkillOwner, m_fRange * 2) )
					{
						pClosest = pTarget;
						fDist = fNextDist;						
					}
				}
			}
		}

		if( pClosest )
		{
			//rotate player, and shoot the new target
			kpuVector vHeading = pClosest->GetLocation() - pSkillOwner->GetLocation();
			vHeading.Normalize();
			pSkillOwner->SetHeading(vHeading);
			Projectile* pBullet = new Projectile(Projectile::ePT_Bullet, m_fDamage, m_fRange, m_eDamageType, pSkillOwner, pSkillOwner->GetLocation(), vHeading);
			g_pGameState->AddActor(pBullet);
		}
	}

	return true;
}
