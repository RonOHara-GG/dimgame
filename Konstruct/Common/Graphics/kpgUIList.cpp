#include "StdAfx.h"
#include "kpgUIList.h"
#include "kpgTexture.h"
#include "kpgRenderer.h"
#include "Common/Graphics/kpgFont.h"

kpgUIList::kpgUIList(void)
{
	m_iRows = 0;
	m_iColumns = 0;
	m_paColumnWidths = 0;
	//m_paRowHeights = 0;
	m_paIcons = 0;
	m_pScrollBar = 0;
	m_pDataSource = 0;
}

kpgUIList::~kpgUIList(void)
{	
	delete m_paColumnWidths;
	//delete m_paRowHeights;

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
	//m_paRowHeights = new kpuFixedArray<float>(m_iRows);

	//get column and row sizes
	const char* szRowHeight = pNode->Attribute("RowHeight");//_strdup(pNode->Attribute("RowHeight"));	 
	if( szRows )	
	{
		m_fRowHeight = (float)atof(szRowHeight);
	//{
	//	char* szRowData = szRowHeight;

	//	int i;
	//	for(i = 0; *szRowData; i++)
	//	{
	//		char* pData = szRowData;
	//		
	//		while(*szRowData && *szRowData != ' ' )	szRowData++;
	//		if( *szRowData == ' ' ) szRowData++;

	//		m_paRowHeights->Add( (float)atof(pData) );
	//		//szRowData++;
	//	}

	//	free(szRowHeight);
	//	
	//	//fill the rest of the column widths in the same pattern
	//	for(int j = i; j < m_iRows; j++)
	//	{
	//		m_paRowHeights->Add(m_paRowHeights->GetElement(j % i));
	//	}
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

	//see if this list has a scroll bar
	const char* pScroll = pNode->Attribute("Scroll");
	if( pScroll )
	{
		u32 uHash = StringHash(pScroll);
		m_pScrollBar = GetChild(uHash);		

		float fHeight = 1.0f / (m_fRowHeight * m_iRows);
		m_pScrollBar->SetHeight(fHeight);

		m_pScrollBar->SetPosition(0.5f, 0.0f + (fHeight * 0.5f) );
		

	}

	TiXmlElement* pChild = pNode->FirstChildElement("Icons");

	if( pChild )
	{
		int iCount = atoi(pChild->Attribute("Count"));
		m_paIcons = new kpuFixedArray<kpgTexture*>(iCount);

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
	const char*** pszDataArray = (const char***)pszData;

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
		

	m_pDataSource = (char***)pszDataArray;	
}

void kpgUIList::Draw(kpgRenderer *pRenderer, const kpRect &rParent)
{
	// Draw the text
	if( m_bVisible )
	{
		kpgUIWindow::Draw(pRenderer, rParent);
		
		//Draw list
		for(int x = 0 ; x < 1; x++)
		{
			for(int y = 0; y < m_iColumns; y++)
			{
				
				//calculate rect
				kpRect rect;
				rect.m_fTop = x * m_fRowHeight * m_rRect.Height();
				rect.m_fBottom = rect.m_fTop + m_fRowHeight;
				rect.m_fLeft = y * m_paColumnWidths->GetElement(y) * m_rRect.Width();
				rect.m_fRight = rect.m_fLeft + m_paColumnWidths->GetElement(y);

				char* pEsc = strstr(m_pDataSource[x][y], "%");

				if( pEsc )
				{
					//get the index of the icon to draw
					int iIcon = atoi(++pEsc);

					pRenderer->DrawQuad2D(rect, m_paIcons->GetElement(iIcon));
				}
				else
				{
					pRenderer->DrawText2D(m_pDataSource[x][y], rect, m_pFont);
				}

			}
		}
		
		
	}
}

int kpgUIList::ContainsEscChar(const char *psz)
{
	int iLen = strlen(psz);

	for(int i = 0; i < iLen; i++ )
	{
		if( psz[i] == '%' )
			return i; 
	}

	return -1;
}