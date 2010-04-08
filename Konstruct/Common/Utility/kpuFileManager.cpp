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

//void kpuFileManager::MakeRelative(const char *szInFile, char *szOutFile, int nOutFileSize)
//{
//	int iLength = strlen(szInFile);
//	int iFilename = 0;
//
//	for(int i = iLength - 1; i > 0; i--)
//	{
//		if(szInFile[i] == '//')
//		{
//			iFileName = i;
//			break;
//		}
//	}
//
//	strcpy_s(szInFile, iLength - iFilename, &szInFile[iFilename]);
//	memset(szInFile[iLength - iFilename], 0, iLength - (iLength - iFilename));
//	strcpy_s(szOutFile, nOutFileSize, szInFile);
//
//
//
//
//}
