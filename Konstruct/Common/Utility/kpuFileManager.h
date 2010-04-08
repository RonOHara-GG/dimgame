#pragma once

class kpuFileManager
{
public:
	kpuFileManager(void);
	~kpuFileManager(void);

	static void SetRootPath(const char* szRoot);
	static bool GetFullFilePath(const char* szInFile, char* szOutFile, int nOutFileSize);
//	static void MakeRelative(const char* szInFile, char* szOutFile, int nOutFileSize);

protected:
	static char		sm_szRootPath[];
};
