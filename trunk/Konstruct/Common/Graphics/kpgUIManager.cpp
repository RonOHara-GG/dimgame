#include "StdAfx.h"
#include "kpgUIManager.h"
#include "kpgRenderer.h"
#include "Common\Input\kpiInputManager.h"
#include "External/tinyxml/tinyxml.h"
#include "kpgUIWindow.h"
#include "Common/Utility/kpuLinkedList.h"

static const u32 s_uHash_StartUp_Window =	0x7992f870;

kpgUIManager::kpgUIManager(void)
{
	m_plWindowList = new kpuLinkedList();
	m_pCurrentWindow = 0;
	/*const char* test = "StartUp Window";

	u32 uHash = StringHash(test);*/
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

	if( doc.LoadFile(szFile) )
	{
		for(TiXmlElement* pElement = doc.FirstChildElement(); pElement != 0; pElement->NextSiblingElement() )
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
			/*switch(button)
			{
			case s_uHash_Left_Mouse:
				{
					break;
				}
			}*/
			

			
			break;
		}
	}

	
	
	return false;
}