#include "stdafx.h"
#include "Common/Utility/kpuBoundingBox.h"
#include "Common/Utility/kpuBoundingSphere.h"
#include "Common/Utility/kpuPlane.h"
#include "Common/Utility/kpuPhysicsObject.h"

namespace kpuCollisionDetection
{
	typedef struct _CollisionData
	{
		float fDeltaTime;
		PhysicsObject* obj;

		kpuVector vVelocity;
		

		bool bCollided;
		float fDist;
		float fTime;


	}CollisionData;

	bool SphereCollision(kpuBoundingSphere& sphere1, kpuBoundingSphere& sphere2)
	{
		return sphere1.Intersects(sphere2);
	}

	bool BoxCollision(kpuBoundingBox& box1, kpuBoundingBox& box2)
	{
		return box1.Intersects(box2);
	}

	bool SpherePlaneCollision(kpuSphere sphere, kpuPlane plane)
	{
		float fDist = kpuVector::Dot(sphere.




	}



}