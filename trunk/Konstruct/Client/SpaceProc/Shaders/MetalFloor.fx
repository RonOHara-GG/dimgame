#include "Noise.fxh"

float4 vRustColor = float4(0.65f, 0.25f, 0.0f, 1.0f);
float4 vMetalColor1 = float4(0.7f, 0.7f, 0.7f, 1.0f);
float4 vMetalColor2 = float4(0.125f, 0.125f, 0.135f, 1.0f);	

float4 MetalFloor(float3 Pos : POSITION) : COLOR
{
	float fRust = max(PerlinNoise2D(Pos.xy, 3.2f, 1.0f, 0.7f, 12), 0);
	float fMetal1 = (PerlinNoise2D(Pos.xy, 2.6f, 1.0f, 0.55f, 4) + 1) * 0.5f;
	float fMetal2 = min(max((PerlinNoise2D(Pos.xy + float2(cos(Pos.y), cos(Pos.y)) - float2(0.25f, 0.5f), 4.2f, 1.0f, 0.75f, 9)), 0), 0.75f);
	
	float4 fRustColor = fRust * vRustColor;
	float4 fMetalColor1 = fMetal1 * vMetalColor1;
	float4 fMetalColor2 = fMetal2 * vMetalColor2;
	
	return fMetalColor1;// + fMetalColor2 + fRustColor;
}