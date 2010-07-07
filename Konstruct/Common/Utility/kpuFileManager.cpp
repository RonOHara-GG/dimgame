#include "StdAfx.h"
#include "Common\Utility\kpuFileManager.h"

char kpuFileManager::sm_szRootPath[512];

kpuFileManager::kpuFileManager(void)
{
}

kpuFileManager::~kpuFileManager(void)
{
}

void kpuFileManager::SetRootPath(const char* szRoot)
{
	memcpy(sm_szRootPath, szRoot, sizeof(sm_szRootPath));
	size_t nLen = strlen(sm_szRootPath);
	if( sm_szRootPath[nLen - 1] != '/' && sm_szRootPath[nLen - 1] != '\\' )
	{
		sm_szRootPath[nLen] = '\\';
		sm_szRootPath[nLen + 1] = 0;
	}
}

bool kpuFileManager::GetFullFilePath(const char* szInFile, char* szOutFile, int nOutFileSize)
{
	// Check to see if the input path is an absolute path
	if( szInFile[1] == ':' )
	{
		// Absolute path, just copy it over
		memcpy(szOutFile, szInFile, nOutFileSize);
	}
	else
	{
		// Relative Path, prepend the root path
		if( (strlen(sm_szRootPath) + strlen(szInFile) + 1) > (size_t)nOutFileSize )
			return false;		// Not enough space

		strcpy_s(szOutFile, nOutFileSize, sm_szRootPath);
		strcat_s(szOutFile, nOutFileSize, szInFile);
	}

	return true;
}

char* kpuFileManager::GetDirectory(const char* szFilename)
{
	size_t nLen = strlen(szFilename);

	for(int i = nLen -1 ; i > -1; i-- )
	{
		if( szFilename[i] == '/' || szFilename[i] == '\\' )
		{			
			char* szDir = (char*)malloc(sizeof(sm_szRootPath));
			memcpy(szDir, szFilename, sizeof(sm_szRootPath));
			szDir[i + 1] = 0;
			return szDir;
		}
	}

	return 0;
}
