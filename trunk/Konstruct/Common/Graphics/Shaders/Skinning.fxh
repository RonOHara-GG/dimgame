#ifndef _SKINNING_FXH_
#define _SKINNING_FXH_

shared float4x4	g_SkinningMatricies[32]	: SKINNINGMATRICIES;

float4x4 SkinMatrix(uint4 vBlendIndices, float4 vBlendWeights  )
{	
	float weight3 = 1.0f - vBlendWeights.z - vBlendWeights.y - vBlendWeights.x;
	float4x4 skinMatrix = (g_SkinningMatricies[vBlendIndices.x] * vBlendWeights.x) + (g_SkinningMatricies[vBlendIndices.y] * vBlendWeights.y) + 
			(g_SkinningMatricies[vBlendIndices.z] * vBlendWeights.z) + (g_SkinningMatricies[vBlendIndices.w] * weight3);	

	return skinMatrix;
}

#endif // _SKINNING_FXH_