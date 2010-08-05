#ifndef _SKINNING_FXH_
#define _SKINNING_FXH_

shared float4x4	g_SkinningMatricies[32]	: SKINNINGMATRICIES;

float4x4 SkinMatrix(uint4 vBlendIndices, float4 vBlendWeights  )
{
	float4x4 skinMatrix = (g_SkinningMatricies[vBlendIndices[0]] * vBlendWeights.x) + (g_SkinningMatricies[vBlendIndices[1]] * vBlendWeights.y) + 
			(g_SkinningMatricies[vBlendIndices[2]] * vBlendWeights.z) + (g_SkinningMatricies[vBlendIndices[3]] * vBlendWeights.w);
	
	return skinMatrix;
}

#endif // _SKINNING_FXH_