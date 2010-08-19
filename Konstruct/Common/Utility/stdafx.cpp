// stdafx.cpp : source file that includes just the standard includes
// Utility.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <stdio.h>
#include "kpuMatrix.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

static char s_szPrintBuffer[4096];

void Printf(const char* pFormat, ...)
{
	va_list args;
	va_start( args, pFormat );

	_vsnprintf_s(s_szPrintBuffer, sizeof(s_szPrintBuffer), sizeof(s_szPrintBuffer), pFormat, args);
	printf(s_szPrintBuffer);
	OutputDebugString(s_szPrintBuffer);
}

// djb2 hash
u32 StringHash(const char* InputString)
{
	u32 hash = 5381;
	int c;

	while( c = *InputString++ )
		hash = ((hash << 5) + hash) + c;

	return hash;
}

u32 StringHash(const char* InputString, const char* IgnoreCharacters)
{
	u32 hash = 5381;
	int c;
	
	size_t ignoreLen = strlen(IgnoreCharacters);

	while( c = *InputString++ )
	{
		bool bIgnore = false;
		for( size_t i = 0; i < ignoreLen; i++ )
		{
			if( c == IgnoreCharacters[i] )
			{
				bIgnore = true;
				break;
			}
		}

		if( !bIgnore )
			hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

kpuVector ParseCSVVector(char* szCSV)
{
	char* szComma = strchr(szCSV, ',');
	szComma[0] = 0;

	kpuVector vLoc;
	vLoc.SetX((float)atof(szCSV));
	szCSV = szComma + 1;
	szComma = strchr(szCSV, ',');
	szComma[0] = 0;

	vLoc.SetY((float)atof(szCSV));
	szCSV = szComma + 1;

	vLoc.SetZ((float)atof(szCSV));

	return vLoc;
}

kpuVector ParseVector4(char* szV)
{
	char* szSpace = strchr(szV, ' ');
	szSpace[0] = 0;

	kpuVector vLoc;
	vLoc.SetX((float)atof(szV));
	szV = szSpace + 1;
	szSpace = strchr(szV, ' ');
	szSpace[0] = 0;

	vLoc.SetY((float)atof(szV));
	szV = szSpace + 1;
	szSpace = strchr(szV, ' ');
	szSpace[0] = 0;

	vLoc.SetZ((float)atof(szV));
	szV = szSpace + 1;

	vLoc.SetW((float)atof(szV));

	while( *szV && *szV != ' ') szV++;
	*szV = 0;	

	return vLoc;

}

kpuMatrix ParseMatrix(char* szM)
{
	kpuVector v = ParseVector4(szM);

	//move down 4 floats
	int i = 0;
	while( i < 4 )
	{		
		if( !(*szM)  )
			i++;
		szM++;
	}
	kpuVector v1 = ParseVector4(szM);

	//move down 4 floats
	i = 0;
	while( i < 4 )
	{		
		if( !(*szM)  )
			i++;
		szM++;
	}
	kpuVector v2 = ParseVector4(szM);

	//move down 4 floats
	i = 0;
	while( i < 4 )
	{		
		if( !(*szM)  )
			i++;
		szM++;
	}
	kpuVector v3 = ParseVector4(szM);

	return kpuMatrix(v, v1, v2, v3);

}