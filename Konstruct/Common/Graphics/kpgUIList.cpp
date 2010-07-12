#include "StdAfx.h"
#include "kpgUIList.h"
#include "kpgTexture.h"
#include "kpgRenderer.h"
#include "Common/Graphics/kpgFont.h"

#define DEFAULT_SCROLL_DELTA 0.2f

kpgUIList::kpgUIList(void)
{
	m_iRows = 0;
	m_iColumns = 0;
	m_paColumnWidths = 0;
	m_paRowHeights = 0;
	m_paIcons = 0;
	m_pScrollBar = 0;
	m_pDataSource = 0;
	m_fScrollDelta = DEFAULT_SCROLL_DELTA;

	m_fViewOffset[0] = 0.0f;
	m_fViewOffset[1] = 0.0f;
}

kpgUIList::~kpgUIList(void)
{	
	delete m_paColumnWidths;
	delete m_paRowHeights;

	for(int i = 0; i < m_paIcons->GetNumElementsUsed(); i++)
	{
		delete m_paIcons->GetElement(i);
	}	

	delete m_paIcons;

	if( m_pDataSource )
	{
		for(int x = 0; x < m_iRows; x++)
		{
			for(int y = 0; y < m_iColumns; y++)
			{
				delete m_pDataSource[x][y];							
			}

			free(m_pDataSource[x]);
		}

		free(m_pDataSource);
	}
}

void kpgUIList::Load(TiXmlElement *pNode)
{
	kpgUIWindow::Load(pNode);

	//Get column and rows
	const char* szRows = pNode->Attribute("Rows");	 
	if( szRows )	
		m_iRows = atoi(szRows);

	const char* szColumns = pNode->Attribute("Columns");	 
	if( szColumns )	
		m_iColumns = atoi(szColumns);

	m_paColumnWidths = new kpuFixedArray<float>(m_iColumns);
	m_paRowHeights = new kpuFixedArray<float>(m_iRows);

	m_fContentHeight = 0.0f;

	//get column and row sizes
	char* szRowHeight = _strdup(pNode->Attribute("RowHeight"));	 
	if( szRows )	
	{	
		char* szRowData = szRowHeight;

		int i;
		for(i = 0; *szRowData; i++)
		{
			char* pData = szRowData;
			
			while(*szRowData && *szRowData != ' ' )	szRowData++;
			if( *szRowData == ' ' ) szRowData++;

			float fHeight = (float)atof(pData);
			m_paRowHeights->Add( fHeight );	
			m_fContentHeight += fHeight;
		}

		free(szRowHeight);

		 //fill the rest of the column widths in the same pattern
		for(int j = i; j < m_iRows; j++)
		{
			float fHeight = m_paRowHeights->GetElement(j % i);
			m_paRowHeights->Add(fHeight);
			m_fContentHeight += fHeight;
		}
		
	}

	char* szColumnWidth = _strdup(pNode->Attribute("ColumnWidth"));	 
	if( szColumnWidth )	
	{
		char* szColumnData = szColumnWidth;

		int i;
		for(i = 0; *szColumnData; i++)
		{
			char* szData = szColumnData;

			while(*szColumnData && *szColumnData != ' ' ) szColumnData++;
			
			if( *szColumnData == ' ' ) szColumnData++;			

			m_paColumnWidths->Add( (float)atof(szData) );			
		}

		free(szColumnWidth);

		 //fill the rest of the column widths in the same pattern
        for(int j = i++; j < m_iColumns; j++)
        {
			m_paColumnWidths->Add(m_paColumnWidths->GetElement(j % i));
        }
		
	}	

	//see if this window has a scroll bar
	const char* pScroll = pNode->Attribute("ScrollVertical");
	if( pScroll )
	{
		u32 uHash = StringHash(pScroll);
		m_pScrollBar = GetChild(uHash);		

		float fHeight = 1.0f / m_fContentHeight;
		m_pScrollBar->SetHeight(fHeight);

		m_pScrollBar->SetPosition(0.5f, 0.0f + (fHeight * 0.5f) );
		
		pScroll = pNode->Attribute("ScrollDelta");
		if( pScroll )
			m_fScrollDelta = (float)atof(pScroll);		

	}	

	TiXmlElement* pChild = pNode->FirstChildElement("Icons");

	if( pChild )
	{
		int iCount = atoi(pChild->Attribute("Count"));
		m_paIcons = new kpuFixedArray<kpgTexture*>(iCount);

		//Get the size of the icons
		char* szIconSize = _strdup(pChild->Attribute("Size"));	 
		if( szIconSize )	
		{
			char* szIconData = szIconSize;

			for(int i = 0; i < 2; i++)
			{
				char* szData = szIconData;

				while(*szIconData && *szIconData != ' ' ) szIconData++;
				
				if( *szIconData == ' ' ) szIconData++;			

				m_fIconSize[i] = (float)atof(szData);			
			}

			free(szIconSize);
			
		}	

		for(TiXmlElement* pIcon = pChild->FirstChildElement(); pIcon != 0 ; pIcon = pIcon->NextSiblingElement() )
		{
			kpgTexture* texture = new kpgTexture();
			const char* pszFile = pIcon->FirstChild()->Value();
			texture->Load(pszFile);

			m_paIcons->Add(texture);
		}
		
	}

	m_pFont = new kpgFont();
	m_pFont->Load(pNode);
	
}

void kpgUIList::SetDataSource(const char* pszData)
{
	m_pDataSource = (char***)pszData;	
}

void kpgUIList::Draw(kpgRenderer *pRenderer, const kpRect &rParent)
{
	// Draw the text
	if( m_bVisible )
	{
		kpgUIWindow::Draw(pRenderer, rParent);
				
		float fY = m_rRect.m_fTop;

		//Draw list only draw rows that fit within the window
		for(int x = 0 ; x < 1 ; x++)
		{
			float fX = m_rRect.m_fLeft;

			for(int y = 0; y < m_iColumns; y++)
			{				
				//calculate cell's rect
				kpRect rect;
				rect.m_fLeft = fX;

				//add width to fX
				fX += m_rRect.Width() * m_paColumnWidths->GetElement(y);
				rect.m_fRight = fX ;
				rect.m_fTop = fY -( m_fViewOffset[1] * m_rRect.Height() );
				rect.m_fBottom = rect.m_fTop +  m_rRect.Height() * m_paRowHeights->GetElement(x);

				//Only Draw if cell is inside of winow
				if( rect.m_fBottom <= m_rRect.m_fBottom && rect.m_fTop >= m_rRect.m_fTop && rect.m_fLeft >= m_rRect.m_fLeft && rect.m_fRight <= m_rRect.m_fRight)
				{				
					char* pEsc = strstr(m_pDataSource[x][y], "%");

					if( pEsc )
					{
						//get the index of the icon to draw
						int iIcon = atoi(++pEsc);

						//calculate icon rect
						rect = GetIconRectangle(rect);

						pRenderer->DrawQuad2D(rect, m_paIcons->GetElement(iIcon));
					}
					else
					{
						m_szText = m_pDataSource[x][y];
						CalculateTextSize();
						rect = GetTextRectangle(rect);
						pRenderer->DrawText2D(m_szText, rect, m_pFont);
					}
				}

			}

			fY +=  m_rRect.Height() * m_paRowHeights->GetElement(x);
		}
		
		
	}
}

kpRect kpgUIList::GetTextRectangle(kpRect& rRect)
{
	float fHalfTextWidth = m_fTextWidth * 0.5f;
	float fHalfTextHeight = m_fTextHeight * 0.5f;
	float fMidX = rRect.m_fLeft + (rRect.Width() * 0.5f);
	float fMidY = rRect.m_fTop + (rRect.Height() * 0.5f);
	return kpRect(fMidX - fHalfTextWidth, fMidX + fHalfTextWidth, fMidY - fHalfTextHeight, fMidY + fHalfTextHeight);
}

kpRect kpgUIList::GetIconRectangle(kpRect& rRect)
{
	float fHalfWidth = m_fIconSize[0] * rRect.Width() * 0.5f;
	float fHalfHeight = m_fIconSize[1] * rRect.Height() * 0.5f;
	float fMidX = rRect.m_fLeft + (rRect.Width() * 0.5f);
	float fMidY = rRect.m_fTop + (rRect.Height() * 0.5f);
	return kpRect(fMidX - fHalfWidth, fMidX + fHalfWidth, fMidY - fHalfHeight, fMidY + fHalfHeight);
}


void kpgUIList::CalculateRectangle(const kpRect& rParent)
{
	kpgUIWindow::CalculateRectangle(rParent);
}

void kpgUIList::ScrollUp()
{
	//move the scroll bar up and adjust offset
	m_fViewOffset[1] -= m_fScrollDelta;

	//make sure it doesn't go below 0.0
	if( m_fViewOffset[1] < 0.0f )
	{
		m_fViewOffset[1] = 0.0f;
		return;
	}

	m_pScrollBar->Move(0.0f, -m_fScrollDelta);
}

void kpgUIList::ScrollDown()
{
	//move the scroll bar up and adjust offset
	m_fViewOffset[1] += m_fScrollDelta;

	//make sure it doesn't go over the height of the content
	if( m_fViewOffset[1] > m_fContentHeight )
	{
		m_fViewOffset[1] = m_fContentHeight;
		return;
	}	

	m_pScrollBar->Move(0.0f, m_fScrollDelta);

}