#include "StdAfx.h"
#include "Item.h"

Item::Item(void)
{
	m_pszDescription = 0;
	m_pszCost = 0;
	m_pszIcon = 0;
}

Item::~Item(void)
{
	free(m_pszDescription);
	free(m_pszCost);
	free(m_pszIcon);
}

void Item::SetSaleDisplay(const char* pszIcon, const char* pszDescription,const char* pszCost )
{
	m_pszDescription = _strdup(pszDescription);
	m_pszCost = _strdup(pszCost);
	m_pszIcon = _strdup(pszIcon);
}