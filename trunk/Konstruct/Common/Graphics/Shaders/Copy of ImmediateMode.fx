shared float4x4 g_mWorldViewProjection	: WORLDVIEWPROJ;
shared float4x4 g_mWorld				: WORLD;

#ifndef MAX_LIGHT_COUNT
#define MAX_LIGHT_COUNT		4
#endif

shared int		g_LightCount					: LIGHTCOUNT;
shared int		g_LightType[MAX_LIGHT_COUNT]	: LIGHTTYPE;
shared float4	g_LightVector[MAX_LIGHT_COUNT]	: LIGHTVECTOR;
shared float4	g_LightColor[MAX_LIGHT_COUNT]	: LIGHTCOLOR;
shared float4	g_AmbientColor					: AMBIENTCOLOR;

float4 VertexLight(float3 vPos, float3 vNrm)
{
	float4 vLightColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	int nCount = min(g_LightCount, MAX_LIGHT_COUNT);
	for( int i = 0; i < nCount; i++ )
	{
		if( g_LightType[i] == 1 )	// Point Light
		{
			float3 vLightDir = vPos - g_LightVector[i];
			float fDistance = length(vLightDir);
			if( fDistance < g_LightVector[i].w )
			{
				vLightDir = normalize(vLightDir);
				float fNdotL = max(dot(vLightDir, vNrm), 0);
				
				vLightColor += g_LightColor[i] * fNdotL;				
			}			
		}
		else						// Directional Light
		{
			float fNdotL = max(dot(g_LightVector[i].xyz, vNrm), 0);
			vLightColor += g_LightColor[i] * fNdotL;
		}
	}
	
	vLightColor += g_AmbientColor;
	
	return vLightColor;
}

texture g_DefaultTexture	: DEFAULTTEXTURE;

sampler DefaultTextureSampler = sampler_state
{
    Texture = <g_DefaultTexture>;
    MinFilter = LINEAR;  
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VSOutput
{
	float4 vPosition		: POSITION;
	float4 vColor			: COLOR0;
	float2 vTexCoord		: TEXCOORD0;
};

VSOutput ImmediateVS(	float4 vPos : POSITION,
						float3 vNormal : NORMAL0,
						float2 vTexCoord : TEXCOORD0 )
{
	VSOutput OUT;
	
	OUT.vPosition = mul(g_mWorldViewProjection, float4(vPos.xyz, 1.0f));
	//OUT.vPosition = mul(float4(vPos.xyz, 1.0f), g_mWorldViewProjection);
	//OUT.vColor = VertexLight(vPos.xyz, normalize(mul(float4(vNormal, 1), g_mWorld).xyz));
	OUT.vColor = float4(abs(vNormal), 1.0f);
	OUT.vTexCoord = vTexCoord;
	
	return OUT;
}

float4 ImmediatePS( VSOutput IN ) : COLOR0
{
	float4 vTexColor = tex2D(DefaultTextureSampler, IN.vTexCoord);
	
	//return vTexColor * IN.vColor;
	float3 vColor = IN.vColor.xyz;//abs(g_mWorldViewProjection[3].xyz);
	vColor = normalize(vColor);
	return float4(vColor, 1.0f);
}

technique DefaultTechnique
{
	pass p0
	{
		VertexShader = compile vs_1_1 ImmediateVS();
		PixelShader = compile ps_2_0 ImmediatePS();
	}
}						