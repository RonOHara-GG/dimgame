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
	m_mUIRenderMatrix.Orthographic(kpgRenderer::GetInstance()->GetScreenWidth(), kpgRenderer::GetInstance()->GetScreenHeight(), 0.0f, 1.0f);
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
	kpuMatrix mIdentity;
	mIdentity.Identity();

	pRenderer->SetProjectionMatrix(m_mUIRenderMatrix);	
	pRenderer->SetViewMatrix(mIdentity);
	pRenderer->SetWorldMatrix(mIdentity);

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
		for(TiXmlElement* pElement = doc.FirstChildElement()->FirstChildElement(); pElement != 0; pElement = pElement->NextSiblingElement() )
		{
			kpgUIWindow* pWindow = new kpgUIWindow();
			pWindow->Load(pElement);
			m_plWindowList->AddTail(pWindow);
		}

		m_pCurrentWindow = (kpgUIWindow*)m_plWindowList->Next()->GetPointer();
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

void kpgUIManager::NewWindow(u32 uHash)
{
	kpgUIWindow* pWindow = GetUIWindow(uHash);
	if( pWindow )
	{
		pWindow->SetVisible(true);

		m_pCurrentWindow->SetVisible(false);
		m_pCurrentWindow = pWindow;
	}
}

u32 kpgUIManager::HandleInputEvent(eInputEventType type, u32 button)
{	
	// TODO: Handle this event
	switch(type)
	{
	case eIET_ButtonClick:
		{		
			if( button == KPIM_BUTTON_0 )
			{
				kpPoint ptMouse = g_pInputManager->GetMouseLoc();
				kpgUIWindow::eHitLocation eHit;
				//Get window
				kpgUIWindow* pWindow = m_pCurrentWindow->HitTest((float)ptMouse.m_iX, (float)ptMouse.m_iY, kpRect(0.0f, kpgRenderer::GetInstance()->GetScreenWidth(), 0.0f, kpgRenderer::GetInstance()->GetScreenHeight()), &eHit);

				if( pWindow )
				{
					//Get the click event
					switch( pWindow->GetClickEvent() )
					{
					case CE_NEW_WINDOW:
						{
							//Change to a new window
							NewWindow(pWindow->ClickEffectedWindow());
							return 0;
						}	
					default:
						return pWindow->GetClickEvent();
					}	
				}
				
			}
			break;
		}	
	}
	
	return IE_NOT_HANDLED;
}