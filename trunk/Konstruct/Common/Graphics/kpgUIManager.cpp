#include "StdAfx.h"
#include "kpgUIManager.h"
#include "kpgRenderer.h"
#include "Common\Input\kpiInputManager.h"
#include "External/tinyxml/tinyxml.h"
#include "kpgUIWindow.h"
#include "kpgUITextInput.h"
#include "kpgUIList.h"
#include "Common/Utility/kpuLinkedList.h"
#include "Common/Utility/kpuFileManager.h"


static const u32 s_uHash_StartUp_Window =	0xe10340e1;

kpgUIManager::kpgUIManager(void)
{
	m_plWindowList = new kpuLinkedList();
	m_lCurrentWindow = kpuLinkedList();
	m_pWinMouseOver = 0;
	 m_pWinMouseLastOver = 0;
	m_pCurrentInput = 0;
	m_mUIRenderMatrix.Orthographic(kpgRenderer::GetInstance()->GetScreenWidth(), kpgRenderer::GetInstance()->GetScreenHeight(), 0.0f, 1.0f);

	m_pDataSourceMap = new kpuMap<char*, char*>;


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

	kpgUIWindow* pWindow = 0;
	kpuLinkedList*	pIter = m_lCurrentWindow.Next();

	while( pIter )
	{
		pWindow = (kpgUIWindow*)pIter->GetPointer();
		pWindow->Draw(pRenderer, kpRect(0.0f, pRenderer->GetScreenWidth(), 0.0f, pRenderer->GetScreenHeight()));
		pIter = pIter->Next();
	}
		
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
			kpgUIWindow* pWindow = kpgUIWindow::Load(pElement, this);

			if( pWindow )
			{
				pWindow->Load(pElement);
				m_plWindowList->AddTail(pWindow);

				if( pWindow->IsVisible() )
					m_lCurrentWindow.AddTail(pWindow);
			}
		}		
	}
	

	return false;
}

kpgUIWindow* kpgUIManager::GetUIWindow(u32 uHash)
{
	kpuLinkedList* pNext = m_plWindowList->Next();

	while( pNext )
	{
		kpgUIWindow* pWindow = (kpgUIWindow*)pNext->GetPointer();

		kpgUIWindow* pChild = pWindow->GetChild(uHash);

		if( pChild )
			return pChild;

		pNext = pNext->Next();
	}

	return 0;
}

void kpgUIManager::OpenUIWindow(u32 uHash, kpuPhysicalObject* pContext)
{
	kpgUIWindow* pWindow = 0;
	kpuLinkedList*	pIter = m_plWindowList->Next();

	while( pIter && !pWindow )
	{
		pWindow = ((kpgUIWindow*)pIter->GetPointer())->GetChild(uHash);

		if( pWindow )		
		{
			pWindow->Open(pContext);

			//add to list
			m_lCurrentWindow.AddTail(pWindow);
		}

		pIter = pIter->Next();
	}		
}

void kpgUIManager::CloseAll()
{
	kpgUIWindow* pWindow = 0;
	kpuLinkedList*	pIter = m_lCurrentWindow.Next();

	while( pIter )
	{
		pWindow = (kpgUIWindow*)pIter->GetPointer();

		kpuLinkedList* pNext = pIter->Next();
		pWindow->SetVisible(false);
		//remove the window from the list
		pIter->SetPointer(0);
		delete pIter;			

		pIter = pNext;
	}
}

void kpgUIManager::CloseUIWindow(u32 uHash)
{
	kpgUIWindow* pWindow = 0;
	kpuLinkedList*	pIter = m_lCurrentWindow.Next();

	while( pIter )
	{
		pWindow = (kpgUIWindow*)pIter->GetPointer();

		if( pWindow->GetHashCode() == uHash )
		{
			pWindow->SetVisible(false);

			//remove the window from the list			
			pIter->SetPointer(0);
			delete pIter;
			
			break;
		}

		pIter = pIter->Next();
	}
}

void kpgUIManager::ToggleUIWindow(u32 uHash)
{
	kpgUIWindow* pWindow = 0;
	kpuLinkedList*	pIter = m_plWindowList->Next();

	while( pIter && !pWindow )
	{
		pWindow = ((kpgUIWindow*)pIter->GetPointer())->GetChild(uHash);

		if( pWindow )	
		{
			if( pWindow->IsVisible() )
			{
				//close the window
				CloseUIWindow(uHash);
			}
			else	
			{
				//add the window to the open list
				pWindow->Open();
				m_lCurrentWindow.AddTail(pWindow);
			}
			break;
		}

		pIter = pIter->Next();
	}

	
}

void kpgUIManager::SetDataSource(char* pszName, char* pData)
{
	char** pOldData = (*m_pDataSourceMap)[pszName];

	if( pOldData )
		*pOldData = pData;
	else
		m_pDataSourceMap->Add(pszName, pData);	
}

char** kpgUIManager::GetDataSource(char* pszName)
{
	return (*m_pDataSourceMap)[pszName];
}

u32 kpgUIManager::HandleInputEvent(eInputEventType type, u32 button)
{	
	kpgUIWindow* pWindow = 0;
	kpuLinkedList*	pIter = m_lCurrentWindow.Next();
	m_pWinMouseOver = 0;

	while( pIter )
	{
		pWindow = ((kpgUIWindow*)pIter->GetPointer());

		u32 uResult = pWindow->HandleInputEvent(type, button);

		if( uResult != IE_NOT_HANDLED)
		{
			m_pWinMouseLastOver = m_pWinMouseOver;
			return uResult;
		}

		pIter = pIter->Next();
	}

	//check for mouse exit/enter events
	if( type == eIET_MouseMove )
	{
		//mouse exit check
		if( m_pWinMouseLastOver )
		{
			if( m_pWinMouseLastOver != m_pWinMouseOver )
			{
				//Get mouse exit event
				switch( m_pWinMouseLastOver->MouseExitEvent() )
				{
				case CE_CLOSE:
					{
						CloseUIWindow(m_pWinMouseLastOver->MouseExitParam());							
						break;
					}						
				}	
			}
		}		

		//Mouse Enter event
		if( m_pWinMouseOver )
		{
			if( m_pWinMouseLastOver != m_pWinMouseOver)
			{
				//Get mouse enter event
				switch( m_pWinMouseOver->MouseEnterEvent() )
				{
				case CE_OPEN:
					{
						OpenUIWindow(m_pWinMouseOver->MouseEnterParam());
						break;
					}						
				}	
			}
		}

		m_pWinMouseLastOver = m_pWinMouseOver;
		return 0;
	}

	m_pWinMouseLastOver = m_pWinMouseOver;
	
	
	return IE_NOT_HANDLED;
}