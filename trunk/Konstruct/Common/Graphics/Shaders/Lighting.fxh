#ifndef _LIGHTING_FXH_
#define _LIGHTING_FXH_

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
				
				vLightColor += g_LightColor[i] * fNdotL * (saturate(1.0f - fDistance / g_LightVector[i].w) ) ;				
			}			
		}
		else						// Directional Light
		{
			float fNdotL = max(dot(-g_LightVector[i].xyz, vNrm), 0);
			vLightColor += g_LightColor[i] * fNdotL;
		}
	}
	
	vLightColor += g_AmbientColor;
	
	return vLightColor;
}


#endif // _LIGHTING_FXH_