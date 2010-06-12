#include "StdAfx.h"
#include "Projectile.h"
#include "Grid.h"
#include "Level.h"
#include "Common/utility/kpuQuadTree.h"
#include "Common/utility/kpuBoundingCapsule.h"



Projectile::Projectile(ProjectileType eType, float fDamage, float fRange, DamageType eDamageType, Actor* pOwner, kpuVector vLocation, kpuVector vDir)
{
	m_eProjectileType = eType;
	m_fDamage = fDamage;
	m_fRange = fRange;
	m_eDamageType = eDamageType;
	m_pOwner = pOwner;
	SetLocation(vLocation);
	m_vHeading = vDir;

	//assign speed
	switch(m_eProjectileType)
	{
	case ePT_Arrow:
		{
			m_fBaseSpeed = ARROW_SPEED;
			break;
		}
	}
}

Projectile::~Projectile(void)
{
}

bool Projectile::Update(float fGameTime)
{
	if( m_fDistTraveled >= m_fRange )
		return false;

	kpuVector vVelocity;
	kpuVector vOldLocation = GetLocation();

	//move the arrow
	float fDist = fGameTime * m_fBaseSpeed;
	vVelocity += GetHeading() * fDist;
	m_fDistTraveled += fDist;

	//see what the arror hits and move it
	kpuBoundingCapsule capsule(vOldLocation, vOldLocation + vVelocity, 0.0f);

	kpuCollisionData data = g_pGameState->GetLevel()->GetQuadTree()->GetClosestCollision(capsule);

	if( data.m_pObject && data.m_pObject != m_pOwner && data.m_pObject != m_pLastHit )
	{
		if(  data.m_pObject->HasFlag(ATTACKABLE) )
		{
			Actor* pTarget = (Actor*)data.m_pObject;

			m_pLastHit = pTarget;
			pTarget->TakeDamage(m_fDamage, m_eDamageType);
			Impact(pTarget->GetLocation());
		}
		else if( data.m_pObject->HasFlag(WALL) )
		{
			if( data.m_fDist <= 0 )			
				return false;

			SetLocation(vOldLocation + (vVelocity * (data.m_fDist / data.m_fVelLength)));
			return true;
		}
	}

	SetLocation(vOldLocation + vVelocity);
	return true;
}

void Projectile::Impact(kpuVector vImpact)
{
	switch(m_eProjectileType)
	{
	case ePT_Arrow:
		{
			break;
		}
	}
}