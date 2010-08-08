#include "StdAfx.h"
#include "kpgShaderManager.h"
#include "kpgShader.h"
#include "kpgRenderer.h"

kpgShaderManager* kpgShaderManager::m_spShaderManager = 0;

kpgShaderManager::kpgShaderManager(void)
{
	m_plShaders = new kpuLinkedList();
}

kpgShaderManager::~kpgShaderManager(void)
{
	while( m_plShaders->Next() )
	{
		delete (kpgShader*)m_plShaders->GetPointer();
		delete m_plShaders;
	}

	delete m_plShaders;
}

kpgShader* kpgShaderManager::LoadShader(const char *pszFile)
{
	kpgShader* pShader = new kpgShader();
	pShader->LoadFromFile(kpgRenderer::GetInstance(),pszFile);
	m_plShaders->AddTail(pShader);
	return pShader;
}

kpgShaderManager*	kpgShaderManager::GetInstance()
{
	if( !m_spShaderManager )
		m_spShaderManager = new kpgShaderManager();
	return m_spShaderManager;
}
