#pragma once

class kpuRegistry
{
public:
	kpuRegistry(void);
	~kpuRegistry(void);

	static void SetBaseKey(const char* szKey);

	static bool GetNubmerValue(const char* szKey, u32& unVal);
	static bool GetStringValue(const char* szKey, char* szValue, int nValueLength);

protected:
	static char		sm_szBaseKey[];
};
