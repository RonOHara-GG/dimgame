#include "StdAfx.h"
#include "kpgUIList.h"
#include "kpgUISlider.h"
#include "kpgTexture.h"
#include "kpgRenderer.h"
#include "Common/Graphics/kpgFont.h"

#define DEFAULT_SCROLL_DELTA 0.2f

kpgUIList::kpgUIList(kpgUIManager* pManager):kpgUIText(pManager)
{
	m_iRows = 0;
	m_iColumns = 0;
	m_paColumnWidths = 0;
	m_paRowHeights = 0;
	m_paIcons = 0;
	m_pScrollBar = 0;
	m_fContentHeight = 0.0f;
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
}

void kpgUIList::Load(TiXmlElement *pNode)
{
	//kpgUIWindow::Load(pNode);

	//Get column and rows
	const char* szRows = pNode->Attribute("Rows");	 
	if( szRows )	
		m_iRows = atoi(szRows);

	const char* szColumns = pNode->Attribute("Columns");	 
	if( szColumns )	
		m_iColumns = atoi(szColumns);

	m_paColumnWidths = new kpuFixedArray<float>(m_iColumns);
	m_paRowHeights = new kpuFixedArray<float>(m_iRows);	

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

	//Get the display height and width of the list
	const char* szDisplayWidth = pNode->Attribute("ListWidth");
	if( szDisplayWidth )
		m_fListDimensions[0] = (float)atof(szDisplayWidth);

	const char* szDisplayHeight = pNode->Attribute("ListHeight");
	if( szDisplayHeight )
		m_fListDimensions[1] = (float)atof(szDisplayHeight);


	//see if this window has a scroll bar
	const char* pScroll = pNode->Attribute("ScrollVertical");
	if( pScroll )
	{
		u32 uHash = StringHash(pScroll);
		m_pScrollBar = (kpgUISlider*)GetChild(uHash);		
		
		pScroll = pNode->Attribute("ScrollDelta");
		if( pScroll )
			m_fScrollDelta = (float)atof(pScroll);		
		
		CalculateScrollHeight();
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



void kpgUIList::GetDataSource()
{
	kpgUIWindow::GetDataSource();

	if( m_pData )
	{
		int i = 0;
		m_iRows = 0;
		m_fContentHeight = 0.0f;
		//Get the height of the data
		while( ((char***)m_pData)[i++][0] )
		{
			m_fContentHeight += m_paRowHeights->GetElement(i);
			m_iRows++;
		}

		//reset scroll
		if( m_pScrollBar )
		{
			CalculateScrollHeight();
			m_pScrollBar->SetSliderPos(0.0f);
			m_fViewOffset[1] = 0.0f;
		}
	}

}

void kpgUIList::Draw(kpgRenderer *pRenderer, const kpRect &rParent)
{
	// Draw the text
	if( m_bVisible )
	{
		kpgUIWindow::Draw(pRenderer, rParent);

		if( m_pData )
		{
			kpRect rListRect = m_rRect;
			rListRect.m_fBottom = rListRect.m_fTop + (m_fListDimensions[1] * m_rRect.Height());
			rListRect.m_fRight = rListRect.m_fLeft + (m_fListDimensions[0] * m_rRect.Width());
					
			float fY = rListRect.m_fTop;

			//Draw list only draw rows that fit within the window and go until no more data or max rows
			for(int x = 0 ; x < m_iRows ; x++)
			{
				float fX = rListRect.m_fLeft;

				for(int y = 0; y < m_iColumns && ((char***)m_pData)[x][y]; y++)
				{				
					//calculate cell's rect
					kpRect rect;
					rect.m_fLeft = fX;

					//add width to fX
					fX += rListRect.Width() * m_paColumnWidths->GetElement(y);
					rect.m_fRight = fX ;
					rect.m_fTop = fY -( m_fViewOffset[1] * rListRect.Height() );
					rect.m_fBottom = rect.m_fTop +  rListRect.Height() * m_paRowHeights->GetElement(x);

					//Only Draw if cell is inside of winow
					if( rect.m_fBottom <= rListRect.m_fBottom && rect.m_fTop >= rListRect.m_fTop && rect.m_fLeft >= rListRect.m_fLeft && rect.m_fRight <= rListRect.m_fRight)
					{				

						if( ((char***)m_pData)[x][y][0] == '%' )
						{
							//get the index of the icon to draw
							int iIcon = atoi(&((char***)m_pData)[x][y][1]);

							//calculate icon rect
							rect = GetIconRectangle(rect);

							pRenderer->DrawQuad2D(rect, m_paIcons->GetElement(iIcon));
						}
						else
						{
							m_szText = ((char***)m_pData)[x][y];
							CalculateTextSize();
							rect = GetTextRectangle(rect);
							pRenderer->DrawText2D(m_szText, rect, m_pFont);
						}
					}

				}

				fY +=  rListRect.Height() * m_paRowHeights->GetElement(x);
			}	
		
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

	m_pScrollBar->MoveSlider(-m_fScrollDelta / m_fContentHeight);
}

void kpgUIList::ScrollDown()
{
	//move the scroll bar up and adjust offset
	m_fViewOffset[1] += m_fScrollDelta;

	//make sure it doesn't go over the height of the content minus the height of the list
	if( m_fViewOffset[1] > m_fContentHeight - 1.0f)
	{
		m_fViewOffset[1] = m_fContentHeight - 1.0f;
		return;
	}	

	m_pScrollBar->MoveSlider(m_fScrollDelta / m_fContentHeight);

}

void kpgUIList::Scroll()
{
	//get scroll bar position
	float fDelta = m_pScrollBar->GetSliderDelta();
	float fScrollPos = m_pScrollBar->GetSliderPos();

	float fMove = fScrollPos /  (m_fContentHeight / m_fScrollDelta);
	int iOffset = (int)fMove;

	if( fMove - iOffset != 0.0f )
	{
		if( fDelta < 0.0f )
			m_fViewOffset[1] = m_fScrollDelta * iOffset;
		else
			m_fViewOffset[1] = m_fScrollDelta * ( iOffset + 1);
	}		

}

void kpgUIList::CalculateScrollHeight()
{
	float fHeight = 1.0f / m_fContentHeight ;

	//height cannot be bigger than 1.0
	if( fHeight > 1.0f )
		fHeight = 1.0f;	

	m_pScrollBar->SetSliderHeight(fHeight);	
}

void kpgUIList::SelectCell(float fX, float fY)
{
	m_iCellClicked[0] = -1;
	m_iCellClicked[1] = -1;

	kpRect rListRect = m_rRect;
	rListRect.m_fBottom = rListRect.m_fTop + (m_fListDimensions[1] * m_rRect.Height());
	rListRect.m_fRight = rListRect.m_fLeft + (m_fListDimensions[0] * m_rRect.Width());

	//see if mouse is in cells
	if( rListRect.m_fBottom >= fY && rListRect.m_fTop <= fY && rListRect.m_fLeft <= fX && rListRect.m_fRight >= fX)
	{
		//calculate the cell hit
		float fDY = rListRect.m_fTop;

		for(int x = 0 ; x < m_iRows ; x++)
		{
			float fDX = rListRect.m_fLeft;
			kpRect rect;
			rect.m_fTop = fDY -( m_fViewOffset[1] * rListRect.Height() );
			rect.m_fBottom = rect.m_fTop +  rListRect.Height() * m_paRowHeights->GetElement(x);

			//check if mouse is in this row
			if( rect.m_fBottom >= fY && rect.m_fTop <= fY )
			{
				m_iCellClicked[1] = x;
				//find column
				for(int y = 0; y < m_iColumns; y++)
				{				
					rect.m_fLeft = fDX - ( m_fViewOffset[0] * rListRect.Width() );

					//add width to fX
					fDX += rListRect.Width() * m_paColumnWidths->GetElement(y);
					rect.m_fRight = fDX ;						

					//see if mouse is in cell
					if( rect.m_fLeft <= fX && rect.m_fRight >= fX)
					{				
						m_iCellClicked[0] = y;							
					}
				}
			}

			fDY +=  rListRect.Height() * m_paRowHeights->GetElement(x);
		}	
	
	}
		
}

u32 kpgUIList::ClickEvent(float fX, float fY)
{
	switch( m_uClickEvent )
	{
	case CE_SELECT_CELL:		
			SelectCell(fX, fY);
			return 0;		
	default:
		return kpgUIWindow::ClickEvent(fX, fY);
	}

}