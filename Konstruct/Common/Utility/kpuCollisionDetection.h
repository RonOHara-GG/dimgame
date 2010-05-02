#pragma once

#include "stdafx.h"
#include "kpuBoundingBox.h"
#include "kpuBoundingSphere.h"
#include "kpuPlane.h"
#include "kpuRay.h"
#include "kpuPhysicalObject.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Graphics/kpgGeometry.h"
#include "Common/Graphics/kpgGeometryInstance.h"
#include "Common/Graphics/kpgVertexBuffer.h"
#include "kpuFixedArray.h"

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

	float MeshCollision(kpgModel* pMesh1, kpgModel* pMesh2)
	{
		//get verts of both meshes
		kpuFixedArray<kpuVector*> mesh1Verts = kpuFixedArray<kpuVector*>(pMesh1->GetInstance(0)->GetGeometry()->GetVertexBuffer()->GetVertexCount());
		kpuFixedArray<kpuVector*> mesh2Verts = kpuFixedArray<kpuVector*>(pMesh2->GetInstance(0)->GetGeometry()->GetVertexBuffer()->GetVertexCount());

		float fDeepest = 0.0f;

		for(int i = 0; i < mesh1Verts.GetNumElements(); i++)
		{
			mesh1Verts.Add((kpuVector*)&pMesh1->GetInstance(0)->GetGeometry()->GetVertexBuffer()->GetPosition(i));
		}
		
		for(int i = 0; i < mesh2Verts.GetNumElements(); i++)
		{
			mesh2Verts.Add((kpuVector*)&pMesh1->GetInstance(0)->GetGeometry()->GetVertexBuffer()->GetPosition(i));
		}

		//create plane from mesh 2 and see if any verts in mesh 1 cross the plane
		//only check every third point since each 3 is on the same triangle
		for(int i = 0; i < mesh2Verts.GetNumElements() - 3; i += 3 )
		{
			kpuPlane plane(*mesh2Verts[i], *mesh2Verts[1 + 1], *mesh2Verts[1 + 2]);

			for(int j = 0; j < mesh1Verts.GetNumElements() - 3; j += 3)
			{
				float fp1Dist = mesh1Verts[j]->Dot( plane.GetNorm() );
				float fp2Dist = mesh1Verts[j + 1]->Dot( plane.GetNorm() );
				float fp3Dist = mesh1Verts[j + 2]->Dot( plane.GetNorm() );

				//make sure all t3 points are on the same side of the plane
				if ( ( fp1Dist > 0 && fp2Dist > 0 && fp3Dist > 0 ) || ( fp1Dist < 0 && fp2Dist < 0 && fp3Dist < 0) )
					continue;

				//find the odd ball point and store its distance
				if ( fp1Dist > 0 )
				{
					if ( fp2Dist > 0 )
					{
						fp3Dist = abs(fp3Dist);
						//then fp3Dist must be the odd ball
						if ( fDeepest == 0.0f || fp3Dist > 0 )
						{
							fDeepest = fp3Dist;
							continue;
						}
					}

					if ( fp3Dist > 0 )
					{
						//must be fp2Dist then
						fp2Dist = abs(fp2Dist);
						
						if ( fDeepest == 0.0f || fp2Dist > 0 )
						{
							fDeepest = fp2Dist;
							continue;
						}
					}

					//must be fp1Dist by default
					fp1Dist = abs(fp1Dist);
						
					if ( fDeepest == 0.0f || fp1Dist > 0 )
					{
						fDeepest = fp1Dist;
						continue;
					}
				}

				//check the negative side if positive was false
				// fp1Dist must be negative if we got this far
				if ( fp2Dist < 0 )
				{
					//then fp3Dist must be the odd ball
					if ( fDeepest == 0.0f || fp3Dist > 0 )
					{
						fDeepest = fp3Dist;
						continue;
					}
				}

				if ( fp3Dist < 0 )
				{
					//must be fp2Dist then					
					if ( fDeepest == 0.0f || fp2Dist > 0 )
					{
						fDeepest = fp2Dist;
						continue;
					}
				}

				//must be fp1Dist by default					
				if ( fDeepest == 0.0f || fp1Dist > 0 )
				{
					fDeepest = fp1Dist;
				}				

			}
		}

		return fDeepest;


	}
}