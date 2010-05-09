#include "StdAfx.h"
#include "Common/Utility/kpuCollisionDetection.h"
#include "Common/Utility/kpuPlane.h"

kpuCollisionData kpuCollisionDetection::SphereVsSphere(kpuVector vPos1, float fRadius1, kpuVector vPos2, float fRadius2)
{
	kpuCollisionData data;

	float fDist = kpuVector::DistanceSquared(vPos1, vPos2);

	data.m_fDist = fDist;

	float fCombinedRadi = (fRadius1 * fRadius1) + (fRadius2 * fRadius2);
	data.m_bCollided = fDist < fCombinedRadi;

	return data;
}

kpuCollisionData kpuCollisionDetection::SphereVsBox(kpuVector vPos, float fRadius, kpuVector vMin, kpuVector vMax)
{
	kpuCollisionData data;
	//Create a plane for each face and check if the sphere crosses the plane

	//left face
	//Normal in +X direction
	kpuPlane plane(vMax, kpuVector(vMax.GetX(), vMax.GetY(), vMin.GetZ(), 0.0f), kpuVector(vMax.GetX(), vMin.GetY(), vMin.GetZ(), 0.0f) );

	float fDist = plane.DistanceToPoint(vPos);

	//Check the distance and make sure the sphere is even within the bound of the plane
	if (fDist < fRadius && vPos.GetZ() - fRadius < vMax.GetZ() && vPos.GetZ() + fRadius > vMin.GetZ() &&  vPos.GetY() - fRadius < vMax.GetY() && vPos.GetY() + fRadius > vMin.GetY() )
	{
		//We collided
		data.m_fDist = fDist;
		data.m_bCollided = true;
		return data;
	}

	//Right side
	//Normal in -X direction
	plane.ReCreate(kpuVector(vMin.GetX(), vMax.GetY(), vMax.GetZ(), 0.0f), kpuVector(vMin.GetX(), vMax.GetY(), vMin.GetZ(), 0.0f), vMin);

	fDist = plane.DistanceToPoint(vPos);

	//Check the distance and make sure the sphere is even within the bound of the plane
	if (fDist < fRadius && vPos.GetZ() - fRadius < vMax.GetZ() && vPos.GetZ() + fRadius > vMin.GetZ() &&  vPos.GetY() - fRadius < vMax.GetY() && vPos.GetY() + fRadius > vMin.GetY() )
	{
		//We collided
		data.m_fDist = fDist;
		data.m_bCollided = true;
		return data;
	}

	//Front side
	// Normal in +Z direction
	plane.ReCreate(vMax,kpuVector(vMin.GetX(), vMax.GetY(), vMax.GetZ(), 0.0f),  kpuVector(vMin.GetX(), vMin.GetY(), vMax.GetZ(), 0.0f));

	fDist = plane.DistanceToPoint(vPos);

	//Check the distance and make sure the sphere is even within the bound of the plane
	if (fDist < fRadius && vPos.GetX() - fRadius < vMax.GetX() && vPos.GetX() + fRadius > vMin.GetX() &&  vPos.GetY() - fRadius < vMax.GetY() && vPos.GetY() + fRadius > vMin.GetY() )
	{
		//We collided
		data.m_fDist = fDist;
		data.m_bCollided = true;
		return data;
	}

	//Back side
	// Normal in -Z direction
	plane.ReCreate(kpuVector(vMax.GetX(), vMax.GetY(), vMin .GetZ(), 0.0f),kpuVector(vMax.GetX(), vMin.GetY(), vMin.GetZ(), 0.0f),  vMin);

	fDist = plane.DistanceToPoint(vPos);

	//Check the distance and make sure the sphere is even within the bound of the plane
	if (fDist < fRadius && vPos.GetX() - fRadius < vMax.GetX() && vPos.GetX() + fRadius > vMin.GetX() &&  vPos.GetY() - fRadius < vMax.GetY() && vPos.GetY() + fRadius > vMin.GetY() )
	{
		//We collided
		data.m_fDist = fDist;
		data.m_bCollided = true;
		return data;
	}

	//Top side
	// Normal in +Y direction
	plane.ReCreate(kpuVector(vMin.GetX(), vMax.GetY(), vMax.GetZ(), 0.0f),vMax,  kpuVector(vMax.GetX(), vMax.GetY(), vMin.GetZ(), 0.0f));

	fDist = plane.DistanceToPoint(vPos);

	//Check the distance and make sure the sphere is even within the bound of the plane
	if (fDist < fRadius && vPos.GetX() - fRadius < vMax.GetX() && vPos.GetX() + fRadius > vMin.GetX() &&  vPos.GetZ() - fRadius < vMax.GetZ() && vPos.GetZ() + fRadius > vMin.GetZ() )
	{
		//We collided
		data.m_fDist = fDist;
		data.m_bCollided = true;
		return data;
	}

	//Bottom side
	// Normal in -Y direction
	plane.ReCreate(kpuVector(vMin.GetX(), vMin.GetY(), vMax.GetZ(), 0.0f), vMin,  kpuVector(vMax.GetX(), vMin.GetY(), vMin.GetZ(), 0.0f));

	fDist = plane.DistanceToPoint(vPos);

	//Check the distance and make sure the sphere is even within the bound of the plane
	if (fDist < fRadius && vPos.GetX() - fRadius < vMax.GetX() && vPos.GetX() + fRadius > vMin.GetX() &&  vPos.GetZ() - fRadius < vMax.GetZ() && vPos.GetZ() + fRadius > vMin.GetZ() )
	{
		//We collided
		data.m_fDist = fDist;
		data.m_bCollided = true;
		return data;
	}


	return data;		
}

kpuCollisionData kpuCollisionDetection::BoxVsBox(kpuVector vMin1, kpuVector vMax1, kpuVector vMin2, kpuVector vMax2)
{
	kpuCollisionData data;
	
	//check to see if two faces line up then check distance between them

	//Left Face && Right Face
	if ( !( vMin1.GetZ() >= vMax2.GetZ() && vMax1.GetZ() <= vMin2.GetZ() && vMin1.GetY() >= vMin2.GetY() && vMax1.GetY() <= vMin2.GetY() ) )
	{
		//Check Left 1 vs Right 2
		if ( vMin2.GetX() < vMax1.GetX() )
		{
			if (vMin1.GetX() < vMax2.GetX () )
			{
				data.m_bCollided = true;
				data.m_fDist = vMin1.GetX() - vMax2.GetX();
				return data;
			}
		}

		//Check Right 1 vs Left 2
		if ( vMax2.GetX() > vMin1.GetX() )
		{
			if ( vMax1.GetX() > vMin2.GetX () )
			{
				data.m_bCollided = true;
				data.m_fDist = vMax1.GetX() - vMin2.GetX();
				return data;
			}
		}

	}

	// Front Face && Back Face
	if ( !( vMin1.GetX() >= vMax2.GetX() && vMax1.GetX() <= vMin2.GetX() && vMin1.GetY() >= vMin2.GetY() && vMax1.GetY() <= vMin2.GetY() ) )
	{
		//Check Front 1 vs Back 2
		if ( vMax2.GetZ() > vMin1.GetZ() )
		{
			if (vMax1.GetZ() > vMin2.GetZ () )
			{
				data.m_bCollided = true;
				data.m_fDist = vMax1.GetZ() - vMin2.GetZ();
				return data;
			}
		}

		//Check Back 1 vs Front 2
		if ( vMin2.GetZ() < vMax1.GetZ() )
		{
			if (vMax2.GetZ() > vMin1.GetZ () )
			{
				data.m_bCollided = true;
				data.m_fDist = vMin1.GetZ() - vMax2.GetZ();
				return data;
			}
		}


	}

	// Top Face && Bottom Face
	if ( !( vMin1.GetX() >= vMax2.GetX() && vMax1.GetX() <= vMin2.GetX() && vMin1.GetZ() >= vMin2.GetZ() && vMax1.GetZ() <= vMin2.GetZ() ) )
	{
		//Check Top 1 vs Bottom 2
		if ( vMax2.GetY() > vMin1.GetY() )
		{
			if (vMax1.GetY() > vMin2.GetY() )
			{
				data.m_bCollided = true;
				data.m_fDist = vMax1.GetY() - vMin2.GetY();
				return data;
			}
		}

		//Check Bottom 1 vs Top 2
		if ( vMin2.GetY() < vMax1.GetY() )
		{
			if (vMax2.GetY() > vMin1.GetY () )
			{
				data.m_bCollided = true;
				data.m_fDist = vMin1.GetY() - vMax2.GetY();
				return data;
			}
		}

	}

	return data;		
}

kpuCollisionData kpuCollisionDetection::BoxVsCapsule(kpuVector vMin, kpuVector vMax, kpuVector vStart, kpuVector vEnd, float fRadius)
{
	kpuCollisionData data;

	//Same as SphereVSBox but now we need to find the point closest to the each face then find the distance from the point to the plane
	//Equation of line segment P = vStart - fDelta(vEnd - vStart)
	// Find point of intersection between line and plane
	// if distance from start to intersecting is <= capsule length we have intersection
	// dist to intersection = ( N dot ( plane.point - vStart ) ) / ( N dot ( vEnd - vStart ) )

	//add the radius to the end, don't need to worry about radius from start since it isn't moving back in time
	float fCapsuleLen = (vEnd - vStart).Length() + fRadius;
	kpuVector vCapsuleNorm = kpuVector::Normalize(vEnd-vStart);
	float fDelta = 0.0f;
	kpuVector vPos;

	//left face
	//Normal in +X direction
	kpuPlane plane(vMax, kpuVector(vMax.GetX(), vMax.GetY(), vMin.GetZ(), 0.0f), kpuVector(vMax.GetX(), vMin.GetY(), vMin.GetZ(), 0.0f) );

	//Check parallel
	float fDenom = ( plane.GetNorm().Dot( vEnd - vStart ) );

	if( fDenom != 0 )
	{
		fDelta = ( plane.GetNorm().Dot(plane.GetPoint() - vStart ) ) / fDenom;

		//Get point of intersection
		vPos = vStart;
		vPos += vCapsuleNorm * fDelta;

		//See if intersection is inside the face
		if( fDelta <= fCapsuleLen && vPos.GetZ() - fRadius < vMax.GetZ() && vPos.GetZ() + fRadius > vMin.GetZ() &&  vPos.GetY() - fRadius < vMax.GetY() && vPos.GetY() + fRadius > vMin.GetY()  )
		{
			//Get depth of penetration
			data.m_fDist = fCapsuleLen - fDelta;
			data.m_bCollided = true;
			return data;
		}
	}

	//Right side
	//Normal in -X direction
	plane.ReCreate(kpuVector(vMin.GetX(), vMax.GetY(), vMax.GetZ(), 0.0f), kpuVector(vMin.GetX(), vMax.GetY(), vMin.GetZ(), 0.0f), vMin);

	//Check parallel
	fDenom = ( plane.GetNorm().Dot( vEnd - vStart ) );

	if( fDenom != 0 )
	{
		fDelta = ( plane.GetNorm().Dot(plane.GetPoint() - vStart ) ) / fDenom;

		//Get point of intersection
		vPos = vStart;
		vPos += vCapsuleNorm * fDelta;

		//Check the distance and make sure the sphere is even within the bound of the plane
		if( fDelta <= fCapsuleLen && vPos.GetZ() - fRadius < vMax.GetZ() && vPos.GetZ() + fRadius > vMin.GetZ() &&  vPos.GetY() - fRadius < vMax.GetY() && vPos.GetY() + fRadius > vMin.GetY() )
		{
			//We collided
			data.m_fDist = fCapsuleLen - fDelta;
			data.m_bCollided = true;
			return data;
		}
	}

	//Front side
	// Normal in +Z direction
	plane.ReCreate(vMax,kpuVector(vMin.GetX(), vMax.GetY(), vMax.GetZ(), 0.0f),  kpuVector(vMin.GetX(), vMin.GetY(), vMax.GetZ(), 0.0f));

	//Check parallel
	fDenom = ( plane.GetNorm().Dot( vEnd - vStart ) );

	if( fDenom != 0 )
	{
		fDelta = ( plane.GetNorm().Dot(plane.GetPoint() - vStart ) ) / fDenom;

		//Get point of intersection
		vPos = vStart;
		vPos += vCapsuleNorm * fDelta;

		//Check the distance and make sure the sphere is even within the bound of the plane
		if( fDelta <= fCapsuleLen && vPos.GetX() - fRadius < vMax.GetX() && vPos.GetX() + fRadius > vMin.GetX() &&  vPos.GetY() - fRadius < vMax.GetY() && vPos.GetY() + fRadius > vMin.GetY() )
		{
			//We collided
			data.m_fDist = fCapsuleLen - fDelta;
			data.m_bCollided = true;
			return data;
		}

	}

	//Back side
	// Normal in -Z direction
	plane.ReCreate(kpuVector(vMax.GetX(), vMax.GetY(), vMin .GetZ(), 0.0f),kpuVector(vMax.GetX(), vMin.GetY(), vMin.GetZ(), 0.0f),  vMin);

	//Check parallel
	fDenom = ( plane.GetNorm().Dot( vEnd - vStart ) );

	if( fDenom != 0 )
	{
		fDelta = ( plane.GetNorm().Dot(plane.GetPoint() - vStart ) ) / fDenom;
		//Get point of intersection
		vPos = vStart;
		vPos += vCapsuleNorm * fDelta;

		//Check the distance and make sure the sphere is even within the bound of the plane
		if( fDelta <= fCapsuleLen && vPos.GetX() - fRadius < vMax.GetX() && vPos.GetX() + fRadius > vMin.GetX() &&  vPos.GetY() - fRadius < vMax.GetY() && vPos.GetY() + fRadius > vMin.GetY() )
		{
			//We collided
			data.m_fDist = fCapsuleLen - fDelta;
			data.m_bCollided = true;
			return data;
		}
	}

	//Top side
	// Normal in +Y direction
	plane.ReCreate(kpuVector(vMin.GetX(), vMax.GetY(), vMax.GetZ(), 0.0f),vMax,  kpuVector(vMax.GetX(), vMax.GetY(), vMin.GetZ(), 0.0f));

	//Check parallel
	fDenom = ( plane.GetNorm().Dot( vEnd - vStart ) );

	if( fDenom != 0 )
	{
		fDelta = ( plane.GetNorm().Dot(plane.GetPoint() - vStart ) ) / fDenom;

		//Get point of intersection
		vPos = vStart;
		vPos += vCapsuleNorm * fDelta;

		//Check the distance and make sure the sphere is even within the bound of the plane
		if( fDelta <= fCapsuleLen && vPos.GetX() - fRadius < vMax.GetX() && vPos.GetX() + fRadius > vMin.GetX() &&  vPos.GetZ() - fRadius < vMax.GetZ() && vPos.GetZ() + fRadius > vMin.GetZ() )
		{
			//We collided
			data.m_fDist = fCapsuleLen - fDelta;
			data.m_bCollided = true;
			return data;
		}
	}

	//Bottom side
	// Normal in -Y direction
	plane.ReCreate(kpuVector(vMin.GetX(), vMin.GetY(), vMax.GetZ(), 0.0f), vMin,  kpuVector(vMax.GetX(), vMin.GetY(), vMin.GetZ(), 0.0f));

	//Check parallel
	fDenom = ( plane.GetNorm().Dot( vEnd - vStart ) );

	if( fDenom != 0 )
	{
		fDelta = ( plane.GetNorm().Dot(plane.GetPoint() - vStart ) ) / fDenom;

		//Get point of intersection
		vPos = vStart;
		vPos += vCapsuleNorm * fDelta;

		//Check the distance and make sure the sphere is even within the bound of the plane
		if( fDelta <= fCapsuleLen && vPos.GetX() - fRadius < vMax.GetX() && vPos.GetX() + fRadius > vMin.GetX() &&  vPos.GetZ() - fRadius < vMax.GetZ() && vPos.GetZ() + fRadius > vMin.GetZ() )
		{
			//We collided
			data.m_fDist = fCapsuleLen - fDelta;
			data.m_bCollided = true;
			return data;
		}

	}



	return data;		
}

kpuCollisionData kpuCollisionDetection::SphereVsCapsule(kpuVector vPos1, float fRadius1, kpuVector vStart, kpuVector vEnd, float fRadius2)
{
	//Equation of line P = vStart - fDelta(vEnd - vStart)
	// when (vPos - P) dot (vEnd - vStart ) = 0 we have point perpendicula to vPos and the line
	// combine and solve for fDelta 
	kpuCollisionData data;
	float denom = kpuVector::DistanceSquared( vEnd - vStart, vEnd - vStart );

	if (denom == 0 )
	{
		data.m_bCollided = false;
		return data;
	}

	float fDelta = ( ( vPos1.GetX() - vStart.GetX())*( vEnd.GetX() - vStart.GetX() ) + ( vPos1.GetY() - vStart.GetY())*( vEnd.GetY() - vStart.GetY() ) + ( vPos1.GetZ() - vStart.GetZ())*( vEnd.GetZ() - vStart.GetZ() ) ) / denom;

	kpuVector vPointOnLine(vStart.GetX() + ( fDelta * (vEnd.GetX() - vStart.GetX() ) ), vStart.GetY() + ( fDelta * (vEnd.GetY() - vStart.GetY() ) ), vStart.GetZ() + ( fDelta * (vEnd.GetZ() - vStart.GetZ() ) ) , 0.0f);

	float fDist = kpuVector::DistanceSquared(vPos1, vPointOnLine);

	data.m_fDist = fDist;

	float fCombinedRadi = (fRadius1 * fRadius1) + (fRadius2 * fRadius2);
	data.m_bCollided = fDist < fCombinedRadi;
	
	return data;		
}

kpuCollisionData kpuCollisionDetection::CapsuleVsCapsule(kpuVector vStart1, kpuVector vEnd1, float fRadius1, kpuVector vStart2, kpuVector vEnd2, float fRadius2)
{
	kpuCollisionData data;

	/*
	* Calculate the cloestes ddistance between two line segments in the capsules.
	* Which is where the dot product of those two points = 0
	* Pa = vStart1 + d1(vEnd1 - vStart)
	* Pb = vStart2 + d2(vEnd2 - vStart2)
	* Pa dot Pb = 0
	* d1 = ( S1S2dE2S2 E2S2dE1S1 - S1S2dE1S1 E2S2dE2S2 ) / ( E1S1dE1S1 E2S2dE2S2 - E2S2dE1S1 E2S2dE1S1 )
	* d2 = ( S1S2dE2S2 + fd1 * E2S2dE1S1 ) / E2S2dE2S2 )
	*/

	float S1S2dE2S2 = (vStart1 - vStart2).Dot(vEnd2- vStart2);
	float E2S2dE1S1 = (vEnd2 - vStart2).Dot(vEnd1 - vStart1);
	float S1S2dE1S1 = (vStart1 - vStart2).Dot(vEnd1 - vStart1);
	float E1S1dE1S1 = (vEnd1 - vStart1).Dot(vEnd1 - vStart1);
	float E2S2dE2S2 = (vEnd2 - vStart2).Dot(vEnd2 - vStart2);

	if ( E1S1dE1S1 * E2S2dE2S2 - E2S2dE1S1 * E2S2dE1S1 == 0 || E2S2dE2S2 == 0 )
	{
		data.m_bCollided = false;
		return data;
	}

	float fd1 = ( S1S2dE2S2 * E2S2dE1S1 - S1S2dE1S1 *E2S2dE2S2 ) / ( E1S1dE1S1 * E2S2dE2S2 - E2S2dE1S1 * E2S2dE1S1 );
	float fd2 = ( S1S2dE2S2 + fd1 * E2S2dE1S1 ) / E2S2dE2S2 ;

	kpuVector vPointLine1(vStart1.GetX() + ( fd1 * (vEnd1.GetX() - vStart1.GetX() ) ), vStart1.GetY() + ( fd1 * (vEnd1.GetY() - vStart1.GetY() ) ), vStart1.GetZ() + ( fd1 * (vEnd1.GetZ() - vStart1.GetZ() ) ) , 0.0f);
	kpuVector vPointLine2(vStart2.GetX() + ( fd2 * (vEnd2.GetX() - vStart2.GetX() ) ), vStart2.GetY() + ( fd2 * (vEnd2.GetY() - vStart2.GetY() ) ), vStart2.GetZ() + ( fd2 * (vEnd2.GetZ() - vStart2.GetZ() ) ) , 0.0f);

	float fDist = kpuVector::DistanceSquared(vPointLine1, vPointLine2);
	float fCombinedRadi = (fRadius1 * fRadius1) + (fRadius2 * fRadius2);

	data.m_fDist = fDist;
	data.m_bCollided = fDist < fCombinedRadi;

	return data;		
}