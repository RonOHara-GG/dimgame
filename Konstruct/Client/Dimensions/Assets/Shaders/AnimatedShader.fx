#include <ShaderGlobals.fxh>
#include <Lighting.fxh>
#include <DefaultTexture.fxh>
#include <Skinning.fxh>

struct VSOutput
{
	float4 vPosition		: POSITION;
	float3 vColor			: COLOR0;
	float2 vUV				: TEXCOORD0;
};

VSOutput DefaultVS(	float3 vPos : POSITION,
					float3 vNrm : NORMAL,
					float2 vUV  : TEXCOORD0,
					uint4 vBlendIndices : BLENDINDICES,
					float4 vBlendWeights : BLENDWEIGHT )
{
	VSOutput OUT;
	
	float4x4 skinMatrix = SkinMatrix(vBlendIndices, vBlendWeights);
	
	float4 pos = mul(float4(vPos.xyz,1),skinMatrix);
	float3 nrm = mul(float4(vNrm.xyz,1),skinMatrix);
	//float4 pos = mul(skinMatrix, float4(vPos.xyz,1));
	//float3 nrm = mul(skinMatrix, float4(vNrm.xyz,1));
	
	// Transform the normal into world space
	float3 vWorldNrm = mul(normalize(nrm), (float3x3)g_mWorld);
	
	// Compute the light color
	OUT.vColor = VertexLight(pos.xyz, normalize(vWorldNrm));
	
	// Transform the position
	OUT.vPosition = mul(float4(pos.xyz, 1), g_mWorldViewProjection);
	
	// Pass through the UV coordinates
	OUT.vUV = vUV;

	//OUT.vColor = vBlendIndices;
		
	return OUT;
}

float4 DefaultPS( VSOutput IN ) : COLOR0
{
	// Sample the texture for the tex color
	float4 vTexColor = tex2D(DefaultTextureSampler, IN.vUV);
	
	// Combine with the vertex color
	return vTexColor * float4(IN.vColor, 1.0f);

	//return float4(IN.vColor, 1.0f);
	
}

technique DefaultTechnique
{
	pass p0
	{		
		VertexShader = compile vs_2_0 DefaultVS();
		PixelShader = compile ps_2_0 DefaultPS();
	}
}