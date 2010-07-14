#pragma once

class Item
{
public:
	Item(void);
	virtual ~Item(void);

	void	SetSaleDisplay(const char* pszIcon, const char* pszDescription,const char* pszCost);	
	char*	GetCostDisplay() { return m_pszCost; }
	char*	GetDescription() { return m_pszDescription; }
	char*	GetIcon() { return m_pszIcon; }

	int		GetCost() { return m_iCost; }

protected:
	char* m_pszDescription;
	char* m_pszCost;
	char* m_pszIcon;

	int m_iCost;
};
