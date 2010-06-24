#include "StdAfx.h"
#include "kpgUIManager.h"
#include "kpgRenderer.h"
#include "Common\Input\kpiInputManager.h"
#include "External/tinyxml/tinyxml.h"
#include "kpgUIWindow.h"
#include "Common/Utility/kpuLinkedList.h"
#include "Common/Utility/kpuFileManager.h"

static const u32 s_uHash_StartUp_Window =	0xe10340e1;

kpgUIManager::kpgUIManager(void)
{
	m_plWindowList = new kpuLinkedList();
	m_pCurrentWindow = 0;

}

kpgUIManager::~kpgUIManager(void)
{
	if( m_plWindowList )
	{
		kpuLinkedList* pNext = m_plWindowList->Next();

		while( pNext )
		{
			kpuLinkedList* pByeBye = pNext->Next();
			delete pNext;
			pNext = pByeBye;		
		}

		delete m_plWindowList;
	}
}

void kpgUIManager::Update()
{
}

void kpgUIManager::Draw(kpgRenderer* pRenderer)
{
	if( m_pCurrentWindow )
		m_pCurrentWindow->Draw(pRenderer, kpRect(0.0f, pRenderer->GetScreenWidth(), 0.0f, pRenderer->GetScreenHeight()));
}

bool kpgUIManager::LoadWindows(const char *szFile)
{
	TiXmlDocument doc;
	char szFileName[2048];
	kpuFileManager::GetFullFilePath(szFile, szFileName, sizeof(szFileName));

	if( doc.LoadFile(szFileName) )
	{
		for(TiXmlElement* pElement = doc.FirstChildElement(); pElement != 0; pElement = pElement->NextSiblingElement() )
		{
			kpgUIWindow* pWindow = new kpgUIWindow();
			pWindow->Load(pElement);
			m_plWindowList->AddTail(pWindow);
		}

		m_pCurrentWindow = GetUIWindow(s_uHash_StartUp_Window);
	}

	

	return false;
}

kpgUIWindow* kpgUIManager::GetUIWindow(u32 uHash)
{
	kpuLinkedList* pNext = m_plWindowList->Next();

	while( pNext )
	{
		kpgUIWindow* pWindow = (kpgUIWindow*)pNext->GetPointer();

		if( pWindow->GetHashCode() == uHash )
			return pWindow;

		pNext = pNext->Next();
	}

	return 0;
}

bool kpgUIManager::HandleInputEvent(eInputEventType type, u32 button)
{	
	// TODO: Handle this event
	switch(type)
	{
	case eIET_ButtonDown:
		{		
			break;
		}
	case eIET_ButtonUp:
		{
			break;
		}
	case eIET_ButtonClick:
		{
			break;
		}
	case eIET_ButtonDoubleClick:
		{
			break;
		}
	}

	
	
	return false;
}