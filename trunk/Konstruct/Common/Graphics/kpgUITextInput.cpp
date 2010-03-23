#include "StdAfx.h"
#include "Common/Graphics/kpgUITextInput.h"
#include "Common/Graphics/kpgRenderer.h"
#include "Common/Graphics/kpgTexture.h"

const float kBlinkSpeed = 0.75f;

kpgUITextInput::kpgUITextInput(void)
{
	m_pCursor = 0;
	m_bBlink = false;
	m_BlinkTimer.SetSeconds(kBlinkSpeed);
	m_szPasswordText = 0;

	m_eType = eWT_TextInput;
}

kpgUITextInput::~kpgUITextInput(void)
{
	if( m_szPasswordText )
		free(m_szPasswordText);
	if( m_pCursor )
		delete m_pCursor;
}

void kpgUITextInput::LoadDefaults()
{
	// Load the default frame
	kpgUIWindow::LoadDefaults();

	// Load the cursor texture
	m_pCursor = new kpgTexture();
	m_pCursor->Load("ScreenLayouts\\System\\Images\\DefaultCursor.dds");
}

void kpgUITextInput::SetPasswordMode(bool bPasswordMode)
{
	if( bPasswordMode && !m_szPasswordText )
	{
		// Password mode requested and not in password mode yet
		
		// Copy the password
		m_szPasswordText = m_szText;

		// Replace the display string with *s
		size_t len = strlen(m_szText);
		m_szText = (char*)malloc(len + 1);
		memset(m_szText, '*', len);
		m_szText[len] = 0;
	}
	else if( !bPasswordMode && m_szPasswordText )
	{
        // We are in password mode and we are requested to not be

		// Free the * string
		free(m_szText);

		// Swap the pointers
		m_szText = m_szPasswordText;
		m_szPasswordText = 0;
	}
}

void kpgUITextInput::SetText(const char* szText)
{
	if( m_szPasswordText )
	{
		// We are in password mode

		// Free the existing password
		free(m_szPasswordText);

		// Free the existing * string
		free(m_szText);

		// Copy the input string
		m_szPasswordText = _strdup(szText);

		// Make a new * string
		size_t len = strlen(szText);
		m_szText = (char*)malloc(len + 1);
		memset(m_szText, '*', len);
		m_szText[len] = 0;
	}	
	else
	{
		// Normal mode, just hand it to the normal set text
		kpgUIText::SetText(szText);
	}
}

void kpgUITextInput::Update()
{
	// Call the base class
	kpgUIText::Update();

	// Update blinker
	if( m_BlinkTimer.Expired() )
	{
		m_bBlink = !m_bBlink;
		m_BlinkTimer.SetSeconds(kBlinkSpeed);
	}
}

void kpgUITextInput::Draw(kpgRenderer* pRenderer, const kpRect& rParent)
{
	// Call the base class to draw the frame and the text
	kpgUIText::Draw(pRenderer, rParent);

	if( m_bVisible && m_pCursor && m_bBlink )
	{
		// TODO: Add Focus Check

		// Draw the cursor
		kpRect rRect = GetTextRectangle();
		rRect.m_fLeft = rRect.m_fRight;// + m_fLeftMargin;
		rRect.m_fRight = rRect.m_fLeft + 3;
		pRenderer->DrawQuad2D(rRect, m_pCursor);
	}
}

void kpgUITextInput::CalculateRectangle(const kpRect& rParent)
{
	kpgUIWindow::CalculateRectangle(rParent);
}