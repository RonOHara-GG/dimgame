#ifndef _NOISE_FXH_
#define _NOISE_FXH_

float PerlinNoise2D(float2 inXY, float fScalar, float fAmplitude, float fPersistance, int nOctaves)
{
	float fNoiseVal = 0.0f;
	float2 localXY = inXY;
	
	for( int i = 0; i < nOctaves; i++ )
	{
		fNoiseVal += noise(localXY) * fAmplitude;
		localXY *= fScalar;
		fAmplitude *= fPersistance;
	}
	
	return fNoiseVal;
}

#endif // _NOISE_FXH_