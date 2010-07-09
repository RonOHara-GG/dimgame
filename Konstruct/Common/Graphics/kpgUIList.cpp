#include "StdAfx.h"
#include "kpgUIList.h"
#include "kpgUIText.h"

kpgUIList::kpgUIList(void)
{
	m_iRows = 0;
	m_iColumns = 0;
	m_fColumnWidth = 0.0f;
	m_fRowHeight = 0.0f;
	m_paListObjects = 0;
}

kpgUIList::~kpgUIList(void)
{
	for(int x = 0; x < m_iRows; x++)
	{
		for(int y = 0; y < m_iColumns; y++)
		{
			delete m_paListObjects[x][y];							
		}

		free(m_paListObjects[x]);
	}

	free(m_paListObjects);
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

	m_paListObjects = (kpgUIText***)malloc(sizeof(kpgUIText*) * m_iRows);

	for(int i = 0; i < m_iRows; i++)
	{
		m_paListObjects[i] = (kpgUIText**)malloc(sizeof(kpgUIText*) * m_iColumns);
	}

	//get column and row sizes
	const char* szRowHeight = pNode->Attribute("RowHeight");	 
	if( szRows )	
		m_fRowHeight = (float)atof(szRowHeight);

	const char* szColumnWidth = pNode->Attribute("ColumnWidth");	 
	if( szColumnWidth )	
		m_fColumnWidth = (float)atof(szColumnWidth);


	//create all new text boxes
	for(int x = 0; x < m_iRows; x++)
	{
		for(int y = 0; y < m_iColumns; y++)
		{
			m_paListObjects[x][y] = new kpgUIText();			
			m_paListObjects[x][y]->SetOrientation(eWO_Center);
			m_paListObjects[x][y]->SetPosition(m_fColumnWidth * y + m_fColumnWidth * 0.5f, m_fRowHeight * x + m_fRowHeight * 0.5f);
			
		}
	}
}

void kpgUIList::SetDataSource(const char* pszData)
{
	const char*** pszDataArray = (const char***)pszData;

	for(int x = 0; x < m_iRows; x++)
	{
		for(int y = 0; y < m_iColumns; y++)
		{
			m_paListObjects[x][y]->SetText(pszDataArray[x][y]);
		}
	}
}

void kpgUIList::Draw(kpgRenderer *pRenderer, const kpRect &rParent)
{
	// Draw the text
	if( m_bVisible )
	{
		kpgUIWindow::Draw(pRenderer, rParent);
		
		//Draw list
		for(int x = 0; x < m_iRows; x++)
		{
			for(int y = 0; y < m_iColumns; y++)
			{
				//don't draw if it is off the window
				if( m_fRowHeight * x < 1.0f )
					m_paListObjects[x][y]->Draw(pRenderer, m_rRect);
			}
		}
		
	}
}