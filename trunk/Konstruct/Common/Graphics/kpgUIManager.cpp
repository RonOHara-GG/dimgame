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

	while( pIter && !pWindow )
	{
		pWindow = ((kpgUIWindow*)pIter->GetPointer())->GetChild(uHash);

		if( pWindow )
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

	while( pIter )
	{
		pWindow = ((kpgUIWindow*)pIter->GetPointer());

		u32 uResult = pWindow->HandleInputEvent(type, button);

		if( uResult != IE_NOT_HANDLED)
			return uResult;

		pIter = pIter->Next();
	}

	//	kpgRenderer* pRenderer = kpgRenderer::GetInstance();

	//	kpPoint ptMouse = g_pInputManager->GetMouseLoc();
	//	kpgUIWindow::eHitLocation eHit;

	//	//Get window
	//	kpgUIWindow* pWindow = m_lCurrentWindow->HitTest((float)ptMouse.m_iX, (float)ptMouse.m_iY, kpRect(0.0f, pRenderer->GetScreenWidth(), 0.0f, pRenderer->GetScreenHeight()), &eHit);
	//

	//	// TODO: Handle this event
	//	switch(type)
	//	{
	//	case eIET_ButtonClick:
	//		{		
	//			if( button == KPIM_BUTTON_0 )
	//			{	

	//				if( pWindow )
	//				{
	//					//Get the click event
	//					switch( pWindow->ClickEvent() )
	//					{
	//					case CE_NEW_WINDOW:
	//						{
	//							//Change to a new window
	//							NewWindow(pWindow->ClickEffectedWindow());
	//							return 0;
	//						}	
	//					case CE_SCROLL_UP:
	//						{
	//							kpgUIList* pList = (kpgUIList*)GetUIWindow(pWindow->ClickEffectedWindow());

	//							if( pList )
	//								pList->ScrollUp();
	//							return 0;
	//						}
	//					case CE_SCROLL_DOWN:
	//						{
	//							kpgUIList* pList = (kpgUIList*)GetUIWindow(pWindow->ClickEffectedWindow());

	//							if( pList )
	//								pList->ScrollDown();
	//							return 0;
	//						}
	//					case CE_CLOSE:
	//						{
	//							CloseUIWindow(m_pWinMouseOver->CloseTarget());
	//							break;
	//						}
	//					case CE_SELECT_CELL:
	//						{
	//							kpgUIList* pList = (kpgUIList*)pWindow;

	//							if( pList )
	//								pList->SelectCell((float)ptMouse.m_iX, (float)ptMouse.m_iY);
	//							return 0;
	//						}
	//					default:
	//						return pWindow->ClickEvent();
	//					}	
	//				}
	//				
	//			}
	//			break;
	//		}	
	//	case eIET_MouseMove:
	//		{
	//			if( m_pWinMouseOver )
	//			{
	//				if( m_pWinMouseOver != pWindow )
	//				{
	//					//Get mouse exit event
	//					switch( m_pWinMouseOver->MouseExitEvent() )
	//					{
	//					case CE_CLOSE:
	//						{
	//							CloseUIWindow(m_pWinMouseOver->CloseTarget());
	//							break;
	//						}						
	//					}	
	//				}
	//			}

	//			m_pWinMouseOver = pWindow;

	//			if( m_pWinMouseOver )
	//			{
	//				//Get mouse enter event
	//				switch( m_pWinMouseOver->MouseEnterEvent() )
	//				{
	//				case CE_OPEN:
	//					{
	//						OpenUIWindow(m_pWinMouseOver->ShowTarget());
	//						break;
	//					}						
	//				}	
	//			}

	//			
	//			
	//		}
	//	case eIET_ButtonUp:
	//		{
	//			if( pWindow  && pWindow->IsVisible() && pWindow->HasFrame() )
	//				return 0;
	//			break;
	//		}
	//	}
	//
	//}
	
	
	return IE_NOT_HANDLED;
}