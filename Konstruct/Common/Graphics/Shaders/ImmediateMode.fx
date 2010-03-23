#include "ShaderGlobals.fxh"
#include "Lighting.fxh"
#include "DefaultTexture.fxh"

float4x4 mWorld : MWORLD;
float4x4 mView : MVIEW;
float4x4 mProj : MPROJ;
float4x4 mWVP : MWVP;

struct VSOutput
{
	float4 vPosition		: POSITION;
	float4 vColor			: COLOR0;
	float4 vTexCoord		: TEXCOORD0;
};

VSOutput ImmediateVS(	float3 vPos : POSITION,
						float3 vNormal : NORMAL,
						float2 vTexCoord : TEXCOORD0 )
{
	VSOutput OUT;
	
	OUT.vPosition = mul(float4(vPos.xyz, 1), g_mWorldViewProjection);
	//OUT.vPosition = float4(vPos.xyz, 1);
	
	//OUT.vPosition = mul(float4(vPos.xyz, 1), mWVP);
		
	OUT.vColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	OUT.vTexCoord = float4(vTexCoord.xy, 0.0f, 0.0f);
	
	return OUT;
}

/*
VSOutput ImmediateVS(	float4 vPos : POSITION,
						float3 vNormal : NORMAL0,
						float2 vTexCoord : TEXCOORD0 )
{
	VSOutput OUT;
	
	float4x4 mWorldView = mul(mWorld, mView);
	float3 wvpos = mul(float4(vPos.xyz, 1), (float3x3)mWorldView);
	OUT.vPosition = mul(float4(wvpos, 1), mProj);
	
	//OUT.vPosition = mul(g_mWorldViewProjection, float4(vPos.xyz, 1.0f));
	//OUT.vPosition = mul(float4(vPos.xyz, 1.0f), g_mWorldViewProjection);
	//OUT.vColor = VertexLight(vPos.xyz, normalize(mul(float4(vNormal, 1), g_mWorld).xyz));
	OUT.vColor = float4(abs(g_mWorldViewProjection[0].xyz), 1.0f);
	OUT.vTexCoord = vTexCoord;
	
	return OUT;
}
*/

float4 ImmediatePS( VSOutput IN ) : COLOR0
{
	float4 vTexColor = tex2D(DefaultTextureSampler, IN.vTexCoord);
	
	
	return vTexColor;// * IN.vColor;
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
}

technique DefaultTechnique
{
	pass p0
	{
		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		//SrcBlend = ONE;
		DestBlend = INVSRCALPHA;
		//DestBlend = ONE;
		VertexShader = compile vs_1_1 ImmediateVS();
		PixelShader = compile ps_2_0 ImmediatePS();
	}
}						