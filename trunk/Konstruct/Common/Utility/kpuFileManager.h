#pragma once

class kpuFileManager
{
public:
	kpuFileManager(void);
	~kpuFileManager(void);

	static void SetRootPath(const char* szRoot);
	static bool GetFullFilePath(const char* szInFile, char* szOutFile, int nOutFileSize);
	static char* GetRoot() { return sm_szRootPath; }
	static char* GetDirectory(const char* szFilename);

protected:
	static char		sm_szRootPath[];
};
