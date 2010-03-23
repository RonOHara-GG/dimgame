// UIEditor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "UIEditor.h"
#include "MainFrm.h"

#include "DisplayFrame.h"
#include "ScriptFrame.h"

#include "External\tinyxml\tinyxml.h"

#include "Common\Utility\kpuFileManager.h"
#include "Common\Utility\kpuRegistry.h"
#include ".\uieditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUIEditorApp

BEGIN_MESSAGE_MAP(CUIEditorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_LOAD, OnFileOpen)
END_MESSAGE_MAP()


// CUIEditorApp construction

CUIEditorApp::CUIEditorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CUIEditorApp object

CUIEditorApp theApp;

// CUIEditorApp initialization

BOOL CUIEditorApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	kpuRegistry::SetBaseKey("Software\\Karma Productions\\Konstruct");
	char szAssetPath[512];
	kpuRegistry::GetStringValue("AssetPath", szAssetPath, sizeof(szAssetPath));
	kpuFileManager::SetRootPath(szAssetPath);

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMDIFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create main MDI frame window
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	// try to load shared MDI menus and accelerator table
	//TODO: add additional member variables and load calls for
	//	additional menu types your application may need
	HINSTANCE hInst = AfxGetResourceHandle();
	m_hDisplayMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_Display));
	m_hScriptMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_Script));
	m_hDisplayAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_Display));
	m_hScriptAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_Script));
	// The main window has been initialized, so show and update it
	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();
	return TRUE;
}


// CUIEditorApp message handlers

int CUIEditorApp::ExitInstance() 
{
	//TODO: handle additional resources you may have added
	if (m_hDisplayMenu != NULL)
		FreeResource(m_hDisplayMenu);
	if (m_hScriptMenu != NULL)
		FreeResource(m_hScriptMenu);
	if (m_hDisplayAccel != NULL)
		FreeResource(m_hDisplayAccel);
	if (m_hScriptAccel != NULL)
		FreeResource(m_hScriptAccel);

	return CWinApp::ExitInstance();
}
void CUIEditorApp::OnFileNew() 
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);

	// Create the script window
	m_pScriptFrame = (CScriptFrame*)pFrame->CreateNewChild(RUNTIME_CLASS(CScriptFrame), IDR_Script, m_hScriptMenu, m_hScriptAccel);

	// Create the display window
	m_pDisplayFrame = (CDisplayFrame*)pFrame->CreateNewChild(RUNTIME_CLASS(CDisplayFrame), IDR_Display, m_hDisplayMenu, m_hDisplayAccel);
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CUIEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CUIEditorApp message handlers


void CUIEditorApp::OnFileSave()
{
	if( m_pScriptFrame && m_pDisplayFrame )
	{
		CDisplayWnd* pDisplayWnd = &m_pDisplayFrame->m_wndView;

		CFileDialog dlg(FALSE, "xml", 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "UI Layouts (*.xml)|*.xml|All Files (*.*)|*.*||");
		if( dlg.DoModal() != IDCANCEL )
		{
			// Create the xml dom
			TiXmlDocument doc;
			TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );
			doc.LinkEndChild(decl);

			// Write the version
			TiXmlElement* version = new TiXmlElement("Version");
			version->SetAttribute("Major", UI_MAJOR_VERSION);
			version->SetAttribute("Minor", UI_MINOR_VERSION);
			doc.LinkEndChild(version);

			// Save the window heirarchy
			pDisplayWnd->SaveLayout(&doc);

			// Save the file
			doc.SaveFile(dlg.GetPathName());
		}
	}
}

void CUIEditorApp::OnFileOpen()
{
	if( m_pScriptFrame && m_pDisplayFrame )
	{
		CDisplayWnd* pDisplayWnd = &m_pDisplayFrame->m_wndView;
		CFileDialog dlg(TRUE, "xml", 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "UI Layouts (*.xml)|*.xml|All Files (*.*)|*.*||");
		if( dlg.DoModal() != IDCANCEL )
		{
			TiXmlDocument doc;
			doc.LoadFile(dlg.GetPathName());

			pDisplayWnd->LoadLayout(&doc);
		}
	}
}