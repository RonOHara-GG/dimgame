#pragma once

#include "stdafx.h"
#include "kpuBoundingBox.h"
#include "kpuBoundingSphere.h"
#include "kpuPlane.h"
#include "kpuRay.h"
#include "kpuPhysicalObject.h"

namespace kpuCollisionDetection
{
	typedef struct _CollisionData
	{
		float fDeltaTime;
		kpuPhysicalObject* obj;

		kpuVector vVelocity;
		

		bool bCollided;
		float fDist;

	}CollisionData;

	float SphereCollision(kpuBoundingSphere& sphere1, kpuBoundingSphere& sphere2)
	{
		return sphere1.Intersects(sphere2);
	}

	bool BoxCollision(kpuBoundingBox& box1, kpuBoundingBox& box2)
	{
		return box1.Intersects(box2);
	}

	bool SpherePlaneCollision(kpuBoundingSphere sphere, kpuPlane plane)
	{
		float fDist = sphere.GetLocation().Dot(plane.GetNorm());

		fDist *= fDist;

		if( fDist < sphere.GetRadius() * sphere.GetRadius() )
			return true;

		return false;
	}

	kpuVector RayPlaneIntersection(kpuRay ray, kpuPlane plane)
	{


		return kpuv_Zero;
	}



}