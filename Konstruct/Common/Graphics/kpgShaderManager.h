#pragma once
#include "Common/Utility/kpuLinkedList.h"

class kpgShader;

class kpgShaderManager
{
public:
	kpgShaderManager(void);
	~kpgShaderManager(void);

	// Singleton
	static kpgShaderManager* kpgShaderManager::GetInstance();

	kpgShader* LoadShader(const char* pszFile);

private:
	static kpgShaderManager*	m_spShaderManager;
	kpuLinkedList*	m_plShaders;
};
