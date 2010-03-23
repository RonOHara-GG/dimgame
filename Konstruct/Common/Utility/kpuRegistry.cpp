#include "StdAfx.h"
#include "Common\Utility\kpuRegistry.h"

char kpuRegistry::sm_szBaseKey[512];

kpuRegistry::kpuRegistry(void)
{
}

kpuRegistry::~kpuRegistry(void)
{
}

void kpuRegistry::SetBaseKey(const char* szKey)
{
	memcpy(sm_szBaseKey, szKey, sizeof(sm_szBaseKey));
}

bool kpuRegistry::GetNubmerValue(const char* szKey, u32& unVal)
{
	HKEY hKey;
	DWORD dwDispAndSize;
	long lVal = RegCreateKeyEx(HKEY_CURRENT_USER, sm_szBaseKey, 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &hKey, &dwDispAndSize);
	if( lVal != ERROR_SUCCESS )
		return false;

	dwDispAndSize = sizeof(DWORD);
	lVal = RegQueryValueEx(hKey, szKey, 0, 0, (BYTE*)&unVal, &dwDispAndSize);
	RegCloseKey(hKey);
	if( lVal != ERROR_SUCCESS )
		return false;

	return true;
}

bool kpuRegistry::GetStringValue(const char* szKey, char* szValue, int nValueLength)
{
	HKEY hKey;
	DWORD dwDispAndSize;
	long lVal = RegCreateKeyEx(HKEY_CURRENT_USER, sm_szBaseKey, 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &hKey, &dwDispAndSize);
	if( lVal != ERROR_SUCCESS )
		return false;

	dwDispAndSize = (DWORD)nValueLength;
	lVal = RegQueryValueEx(hKey, szKey, 0, 0, (BYTE*)szValue, &dwDispAndSize);
	RegCloseKey(hKey);
	if( lVal != ERROR_SUCCESS )
		return false;

	return true;
}
