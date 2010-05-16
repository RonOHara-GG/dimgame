#include "StdAfx.h"
#include "kpuCollisionDetection.h"


void kpuCollisionDetection::SphereVsSphere(kpuVector vPos1, float fRadius1, kpuVector vPos2, float fRadius2, kpuCollisionData &data)
{
	float fDist = kpuVector::DistanceSquared(vPos1, vPos2);

	data.m_fDist = fDist;

	float fCombinedRadi = (fRadius1 * fRadius1) + (fRadius2 * fRadius2);
	data.m_bCollided = fDist < fCombinedRadi;	
}

void kpuCollisionDetection::SphereVsBox(kpuVector vPos, float fRadius, kpuVector vMin, kpuVector vMax, kpuCollisionData &data)
{	
	//Create a plane for each face and check if the sphere crosses the plane

	//left face
	//Normal in +X direction
	kpuVector vFaceNormal = kpuv_OneX;
	float fDist = vFaceNormal.Dot(vPos - vMax);

	//Check the distance and make sure the sphere is even within the bound of the plane
	if (fDist * fDist < fRadius * fRadius && vPos.GetZ() - fRadius < vMax.GetZ() && vPos.GetZ() + fRadius > vMin.GetZ() &&  vPos.GetY() - fRadius < vMax.GetY() && vPos.GetY() + fRadius > vMin.GetY() )
	{
		//We collided
		data.m_fDist = fDist;
		data.m_bCollided = true;
		return;
	}

	//Right side
	//Normal in -X direction
	vFaceNormal = -kpuv_OneX;
	fDist = vFaceNormal.Dot(vPos - vMin);

	//Check the distance and make sure the sphere is even within the bound of the plane
	if (fDist * fDist < fRadius * fRadius && vPos.GetZ() - fRadius < vMax.GetZ() && vPos.GetZ() + fRadius > vMin.GetZ() &&  vPos.GetY() - fRadius < vMax.GetY() && vPos.GetY() + fRadius > vMin.GetY() )
	{
		//We collided
		data.m_fDist = fDist;
		data.m_bCollided = true;
		return;
	}

	//Front side
	// Normal in +Z direction
	vFaceNormal = kpuv_OneZ;
	fDist = vFaceNormal.Dot(vPos - vMax);

	//Check the distance and make sure the sphere is even within the bound of the plane
	if (fDist * fDist < fRadius * fRadius && vPos.GetX() - fRadius < vMax.GetX() && vPos.GetX() + fRadius > vMin.GetX() &&  vPos.GetY() - fRadius < vMax.GetY() && vPos.GetY() + fRadius > vMin.GetY() )
	{
		//We collided
		data.m_fDist = fDist;
		data.m_bCollided = true;
		return;
	}

	//Back side
	// Normal in -Z direction	
	vFaceNormal = -kpuv_OneZ;
	fDist = vFaceNormal.Dot(vPos - vMin);

	//Check the distance and make sure the sphere is even within the bound of the plane
	if (fDist * fDist < fRadius * fRadius && vPos.GetX() - fRadius < vMax.GetX() && vPos.GetX() + fRadius > vMin.GetX() &&  vPos.GetY() - fRadius < vMax.GetY() && vPos.GetY() + fRadius > vMin.GetY() )
	{
		//We collided
		data.m_fDist = fDist;
		data.m_bCollided = true;
		return;
	}

	//Top side
	// Normal in +Y direction
	vFaceNormal = kpuv_OneY;
	fDist = vFaceNormal.Dot(vPos - vMax);

	//Check the distance and make sure the sphere is even within the bound of the plane
	if (fDist * fDist < fRadius * fRadius && vPos.GetX() - fRadius < vMax.GetX() && vPos.GetX() + fRadius > vMin.GetX() &&  vPos.GetZ() - fRadius < vMax.GetZ() && vPos.GetZ() + fRadius > vMin.GetZ() )
	{
		//We collided
		data.m_fDist = fDist;
		data.m_bCollided = true;
		return;
	}

	//Bottom side
	// Normal in -Y direction
	vFaceNormal = -kpuv_OneY;
	fDist = vFaceNormal.Dot(vPos - vMin);

	//Check the distance and make sure the sphere is even within the bound of the plane
	if (fDist * fDist < fRadius * fRadius && vPos.GetX() - fRadius < vMax.GetX() && vPos.GetX() + fRadius > vMin.GetX() &&  vPos.GetZ() - fRadius < vMax.GetZ() && vPos.GetZ() + fRadius > vMin.GetZ() )
	{
		//We collided
		data.m_fDist = fDist;
		data.m_bCollided = true;
		return;
	}	
}

void kpuCollisionDetection::BoxVsBox(kpuVector vMin1, kpuVector vMax1, kpuVector vMin2, kpuVector vMax2, kpuCollisionData &data)
{	
	//check to see if two faces line up then check distance between them

	//Left Face && Right Face
	if ( vMin1.GetZ() < vMax2.GetZ() && vMax1.GetZ() > vMin2.GetZ() && vMin1.GetY() < vMax2.GetY() && vMax1.GetY() > vMin2.GetY()  )
	{
		//Check Left 1 vs Right 2
		if ( vMin2.GetX() < vMax1.GetX() && vMax1.GetX() >= vMax2.GetX())
		{
			if (vMin1.GetX() < vMax2.GetX () )
			{
				data.m_bCollided = true;
				data.m_fDist = data.m_fVelLength - ( vMax2.GetX() - vMin1.GetX() );
				return;
			}
		}

		//Check Right 1 vs Left 2
		if ( vMax2.GetX() > vMin1.GetX() && vMin1.GetX() <= vMin2.GetX() )
		{
			if ( vMax1.GetX() > vMin2.GetX () )
			{
				data.m_bCollided = true;
				data.m_fDist =  data.m_fVelLength - ( vMax1.GetX() - vMin2.GetX() );
				return;
			}
		}

	}

	// Front Face && Back Face
	if ( vMin1.GetX() < vMax2.GetX() && vMax1.GetX() > vMin2.GetX() && vMin1.GetY() < vMax2.GetY() && vMax1.GetY() > vMin2.GetY() )
	{
		//Check Front 1 vs Back 2		
		if ( vMin2.GetZ() < vMax1.GetZ() && vMin1.GetZ() <= vMin2.GetZ() )
		{
			if (vMax1.GetZ() > vMin2.GetZ () )
			{
				data.m_bCollided = true;
				data.m_fDist = data.m_fVelLength - ( vMax1.GetZ() - vMin2.GetZ() );
				return;
			}
		}

		//Check Back 1 vs Front 2
		if ( vMax2.GetZ() > vMin1.GetZ() && vMax1.GetZ() >= vMax2.GetZ() )
		{
			if (vMin1.GetZ() < vMax2.GetZ () )
			{
				data.m_bCollided = true;
				data.m_fDist = data.m_fVelLength - (vMax2.GetZ() - vMin1.GetZ());
				return;
			}
		}


	}

	// Top Face && Bottom Face
	if ( vMin1.GetX() < vMax2.GetX() && vMax1.GetX() > vMin2.GetX() && vMin1.GetZ() < vMax2.GetZ() && vMax1.GetZ() > vMin2.GetZ() )
	{
		//Check Top 1 vs Bottom 2
		if ( vMax2.GetY() > vMin1.GetY() && vMin1.GetY() <= vMin2.GetY() )
		{
			if (vMax1.GetY() > vMin2.GetY() )
			{
				data.m_bCollided = true;
				data.m_fDist = data.m_fVelLength - ( vMax1.GetY() - vMin2.GetY());
				return;
			}
		}

		//Check Bottom 1 vs Top 2
		if ( vMin2.GetY() < vMax1.GetY() && vMax1.GetY() >= vMax2.GetY() )
		{
			if (vMax2.GetY() > vMin1.GetY () )
			{
				data.m_bCollided = true;
				data.m_fDist = data.m_fVelLength - (vMax2.GetY() - vMin1.GetY());
				return;
			}
		}

	}

	//Check if completly inside other aabb
	if(vMin1.GetX() > vMin2.GetX() && vMin1.GetY() > vMin2.GetY() && vMin1.GetZ() > vMin2.GetZ() && vMax1.GetX() < vMax2.GetX() && vMax1.GetY() < vMax2.GetY() && vMax1.GetZ() < vMax2.GetZ() )
	{
		data.m_bCollided = true;
		data.m_fDist = 0.0f;
		return;
	}
		
}

void kpuCollisionDetection::BoxVsCapsule(kpuVector vMin, kpuVector vMax, kpuVector vStart, kpuVector vEnd, float fRadius, kpuCollisionData &data)
{
	//Same as SphereVSBox but now we need to find the point closest to the each face then find the distance from the point to the plane
	//Equation of line segment P = vStart - fDelta(vEnd - vStart)
	// Find point of intersection between line and plane
	// if distance from start to intersecting is <= capsule length we have intersection
	// dist to intersection = ( N dot ( plane.point - vStart ) ) / ( N dot ( vEnd - vStart ) )

	//add the radius to the end, don't need to worry about radius from start since it isn't moving back in time
	float fDelta = 0.0f;
	float fDist = 0.0f;
	kpuVector vPos;
	kpuVector vFaceNormal;

	//right face
	//Normal in +X direction
	vFaceNormal = kpuv_OneX;

	//Check parallel
	float fDenom = ( vFaceNormal.Dot( vEnd - vStart ) );

	if( fDenom != 0 )
	{
		fDelta = ( vFaceNormal.Dot(vMax - vStart ) ) / fDenom;

		fDist = 0.0f;

		//Get point of intersection
		if( fDelta > 1.0f )	
		{
			//see if it intersects
			fDist = vFaceNormal.Dot(vEnd - vMax);			
			vPos = vEnd;			
		}
		else if(fDelta < 0.0f)
		{		
			fDist = vFaceNormal.Dot(vStart - vMax);
			vPos = vStart;			
		}
		else
		{		
			vPos = vStart;
			vPos += (vEnd - vStart) * fDelta;
			fDist = sqrt(kpuVector::DistanceSquared(vPos, vStart));
		}

		//See if intersection is inside the face
		if( vPos.GetZ() - fRadius < vMax.GetZ() && vPos.GetZ() + fRadius > vMin.GetZ() &&  vPos.GetY() - fRadius < vMax.GetY() && vPos.GetY() + fRadius > vMin.GetY()  )
		{
			if( fDist * fDist <= fRadius * fRadius )
			{
				data.m_fDist = 0.0f;
				data.m_bCollided = true;
				return;
			}
			else if( fDelta >= 0 && fDelta <= 1)
			{
				data.m_fDist = abs(fDist) - fRadius;
				data.m_bCollided = true;
				return;
			}			
		}
	}

	//Left side
	//Normal in -X direction	
	vFaceNormal = -kpuv_OneX;

	//Check parallel
	fDenom = ( vFaceNormal.Dot( vEnd - vStart ) );

	if( fDenom != 0 )
	{
		fDelta = ( vFaceNormal.Dot(vMin - vStart ) ) / fDenom;
		float fDist = 0.0f;

		//Get point of intersection
		if( fDelta > 1.0f )	
		{
			//see if it intersects
			fDist = vFaceNormal.Dot(vEnd - vMin);
			vPos = vEnd;			
		}
		else if(fDelta < 0.0f)
		{			
			//see if it intersects
			fDist = vFaceNormal.Dot(vStart - vMin);
			vPos = vStart;			
		}
		else
		{		
			vPos = vStart;
			vPos += (vEnd - vStart) * fDelta;
			fDist = sqrt(kpuVector::DistanceSquared(vPos, vStart));
		}

		//Check the distance and make sure the sphere is even within the bound of the plane
		if( vPos.GetZ() - fRadius < vMax.GetZ() && vPos.GetZ() + fRadius > vMin.GetZ() &&  vPos.GetY() - fRadius < vMax.GetY() && vPos.GetY() + fRadius > vMin.GetY() )
		{
			if( fDist * fDist <= fRadius * fRadius )
			{
				data.m_fDist = 0.0f;
				data.m_bCollided = true;
				return;
			}
			else if( fDelta >= 0 && fDelta <= 1)
			{
				data.m_fDist = abs(fDist) - fRadius;
				data.m_bCollided = true;
				return;
			}	
		}
	}

	//Front side
	// Normal in +Z direction
	vFaceNormal = kpuv_OneZ;

	//Check parallel
	fDenom = ( vFaceNormal.Dot( vEnd - vStart ) );
	
	if( fDenom != 0 )
	{
		fDelta = ( vFaceNormal.Dot(vMax - vStart ) ) / fDenom;
		float fDist = 0.0f;

		//Get point of intersection
		if( fDelta > 1.0f )	
		{
			//see if it intersects
			fDist = vFaceNormal.Dot(vEnd - vMax);
			vPos = vEnd;			
		}
		else if(fDelta < 0.0f)
		{			
			//see if it intersects
			fDist = vFaceNormal.Dot(vStart - vMax);
			vPos = vStart;			
		}
		else
		{		
			vPos = vStart;
			vPos += (vEnd - vStart) * fDelta;	
			fDist = sqrt(kpuVector::DistanceSquared(vPos, vStart));
		}

		//Check the distance and make sure the sphere is even within the bound of the plane
		if(  vPos.GetX() - fRadius < vMax.GetX() && vPos.GetX() + fRadius > vMin.GetX() &&  vPos.GetY() - fRadius < vMax.GetY() && vPos.GetY() + fRadius > vMin.GetY() )
		{
			if( fDist * fDist <= fRadius * fRadius )
			{
				data.m_fDist = 0.0f;
				data.m_bCollided = true;
				return;
			}
			else if( fDelta >= 0 && fDelta <= 1)
			{
				data.m_fDist = abs(fDist) - fRadius;
				data.m_bCollided = true;
				return;
			}	
		}

	}

	//Back side
	// Normal in -Z direction
	vFaceNormal = -kpuv_OneZ;

	//Check parallel
	fDenom = ( vFaceNormal.Dot( vEnd - vStart ) );

	if( fDenom != 0 )
	{
		fDelta = ( vFaceNormal.Dot(vMin - vStart ) ) / fDenom;
		float fDist = 0.0f;

			//Get point of intersection
		if( fDelta > 1.0f )	
		{
			//see if it intersects
			fDist = vFaceNormal.Dot(vEnd - vMin);
			vPos = vEnd;			
		}
		else if(fDelta < 0.0f)
		{			
			//see if it intersects
			fDist = vFaceNormal.Dot(vStart - vMin);
			vPos = vStart;			
		}
		else
		{		
			vPos = vStart;
			vPos += (vEnd - vStart) * fDelta;	
			fDist = sqrt(kpuVector::DistanceSquared(vPos, vStart));
		}

		//Check the distance and make sure the sphere is even within the bound of the plane
		if( vPos.GetX() - fRadius < vMax.GetX() && vPos.GetX() + fRadius > vMin.GetX() &&  vPos.GetY() - fRadius < vMax.GetY() && vPos.GetY() + fRadius > vMin.GetY() )
		{
			if( fDist * fDist <= fRadius * fRadius )
			{
				data.m_fDist = 0.0f;
				data.m_bCollided = true;
				return;
			}
			else if( fDelta >= 0 && fDelta <= 1)
			{
				data.m_fDist = abs(fDist) - fRadius;
				data.m_bCollided = true;
				return;
			}	
		}
	}

	//Top side
	// Normal in +Y direction
	vFaceNormal = kpuv_OneY;

	//Check parallel
	fDenom = ( vFaceNormal.Dot( vEnd - vStart ) );

	if( fDenom != 0 )
	{
		fDelta = ( vFaceNormal.Dot(vMax - vStart ) ) / fDenom;
		float fDist = 0.0f;
			//Get point of intersection
		if( fDelta > 1.0f )	
		{
			//see if it intersects
			fDist = vFaceNormal.Dot(vEnd - vMax);
			vPos = vEnd;			
		}
		else if(fDelta < 0.0f)
		{			
			//see if it intersects
			fDist = vFaceNormal.Dot(vStart - vMax);
			vPos = vStart;			
		}
		else
		{		
			vPos = vStart;
			vPos += (vEnd - vStart) * fDelta;	
			fDist = sqrt(kpuVector::DistanceSquared(vPos, vStart));
		}

		//Check the distance and make sure the sphere is even within the bound of the plane
		if( vPos.GetX() - fRadius < vMax.GetX() && vPos.GetX() + fRadius > vMin.GetX() &&  vPos.GetZ() - fRadius < vMax.GetZ() && vPos.GetZ() + fRadius > vMin.GetZ() )
		{
			if( fDist * fDist <= fRadius * fRadius )
			{
				data.m_fDist = 0.0f;
				data.m_bCollided = true;
				return;
			}
			else if( fDelta >= 0 && fDelta <= 1)
			{
				data.m_fDist = abs(fDist) - fRadius;
				data.m_bCollided = true;
				return;
			}	
		}
	}

	//Bottom side
	// Normal in -Y direction
	vFaceNormal = -kpuv_OneY;

	//Check parallel
	fDenom = ( vFaceNormal.Dot( vEnd - vStart ) );

	if( fDenom != 0 )
	{
		fDelta = ( vFaceNormal.Dot(vMin - vStart ) ) / fDenom;
		float fDist = 0.0f;

			//Get point of intersection
		if( fDelta > 1.0f )	
		{
			//see if it intersects
			float fDist = vFaceNormal.Dot(vEnd - vMin);
			vPos = vEnd;			
		}
		else if(fDelta < 0.0f)
		{			
			//see if it intersects
			float fDist = vFaceNormal.Dot(vStart - vMin);
			vPos = vStart;			
		}
		else
		{		
			vPos = vStart;
			vPos += (vEnd - vStart) * fDelta;	
			fDist = sqrt(kpuVector::DistanceSquared(vPos, vStart));
		}

		//Check the distance and make sure the sphere is even within the bound of the plane
		if( vPos.GetX() - fRadius < vMax.GetX() && vPos.GetX() + fRadius > vMin.GetX() &&  vPos.GetZ() - fRadius < vMax.GetZ() && vPos.GetZ() + fRadius > vMin.GetZ() )
		{
			if( fDist * fDist <= fRadius * fRadius )
			{
				data.m_fDist = 0.0f;
				data.m_bCollided = true;
				return;
			}
			else if( fDelta >= 0 && fDelta <= 1)
			{
				data.m_fDist = abs(fDist) - fRadius;
				data.m_bCollided = true;
				return;
			}	
		}

	}	
}

void kpuCollisionDetection::SphereVsCapsule(kpuVector vPos1, float fRadius1, kpuVector vStart, kpuVector vEnd, float fRadius2, kpuCollisionData &data)
{
	//Equation of line P = vStart - fDelta(vEnd - vStart)
	// when (vPos - P) dot (vEnd - vStart ) = 0 we have point perpendicula to vPos and the line
	// combine and solve for fDelta 	
	float denom = (vEnd - vStart).Dot(vEnd - vStart );

	if (denom == 0 )
	{
		data.m_bCollided = false;
		return;
	}

	float fDelta = ( ( vPos1.GetX() - vStart.GetX())*( vEnd.GetX() - vStart.GetX() ) + ( vPos1.GetY() - vStart.GetY())*( vEnd.GetY() - vStart.GetY() ) + ( vPos1.GetZ() - vStart.GetZ())*( vEnd.GetZ() - vStart.GetZ() ) ) / denom;

	float fDist = 0.0f;

	if( fDelta < 0 )	
		fDist = kpuVector::DistanceSquared(vStart, vPos1);
	else if( fDelta <= 1 )
	{
		kpuVector vPointOnLine(vStart.GetX() + ( fDelta * (vEnd.GetX() - vStart.GetX() ) ), vStart.GetY() + ( fDelta * (vEnd.GetY() - vStart.GetY() ) ), vStart.GetZ() + ( fDelta * (vEnd.GetZ() - vStart.GetZ() ) ) , 0.0f);

		fDist = kpuVector::DistanceSquared(vPos1, vPointOnLine);
	}
	else	
		fDist = kpuVector::DistanceSquared(vEnd, vPos1);

	data.m_fDist = fDist;

	float fCombinedRadi = (fRadius1 * fRadius1) + (fRadius2 * fRadius2);
	data.m_bCollided = fDist < fCombinedRadi;
	
	return;		
}

void kpuCollisionDetection::CapsuleVsCapsule(kpuVector vStart1, kpuVector vEnd1, float fRadius1, kpuVector vStart2, kpuVector vEnd2, float fRadius2, kpuCollisionData &data)
{
	/*
	* Calculate the cloestes ddistance between two line segments in the capsules.
	* Which is where the dot product of those two points = 0
	* Pa = vStart1 + d1(vEnd1 - vStart)
	* Pb = vStart2 + d2(vEnd2 - vStart2)
	* Pa dot Pb = 0
	* d = Dot, S = Start, E = End, d1 = delta1, d2 = delta2
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
		return;
	}

	float fd1 = ( S1S2dE2S2 * E2S2dE1S1 - S1S2dE1S1 *E2S2dE2S2 ) / ( E1S1dE1S1 * E2S2dE2S2 - E2S2dE1S1 * E2S2dE1S1 );
	float fd2 = ( S1S2dE2S2 + fd1 * E2S2dE1S1 ) / E2S2dE2S2 ;
	kpuVector vPointLine1, vPointLine2;

	if(fd2 >= 0 && fd2 <= 1)		
		vPointLine2 = kpuVector(vStart2.GetX() + ( fd2 * (vEnd2.GetX() - vStart2.GetX() ) ), vStart2.GetY() + ( fd2 * (vEnd2.GetY() - vStart2.GetY() ) ), vStart2.GetZ() + ( fd2 * (vEnd2.GetZ() - vStart2.GetZ() ) ) , 0.0f);
	else if(fd2 < 0 )
		vPointLine2 = vStart2;
	else if(fd2 > 1 )
		vPointLine2 = vEnd2;

	if ( fd1 >= 0 && fd1 <= 1  )	
		vPointLine1 = kpuVector(vStart1.GetX() + ( fd1 * (vEnd1.GetX() - vStart1.GetX() ) ), vStart1.GetY() + ( fd1 * (vEnd1.GetY() - vStart1.GetY() ) ), vStart1.GetZ() + ( fd1 * (vEnd1.GetZ() - vStart1.GetZ() ) ) , 0.0f);
	else if(fd1 < 0 )
		vPointLine1 = vStart1;
	else if(fd1 > 0)
		vPointLine1 = vEnd1;
		
	float fDist = kpuVector::DistanceSquared(vPointLine1, vPointLine2);
	float fCombinedRadi = (fRadius1 * fRadius1) + (fRadius2 * fRadius2);

	data.m_fDist = fDist;
	data.m_bCollided = fDist < fCombinedRadi;
}