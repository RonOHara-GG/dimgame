#pragma once
#include "External/tinyxml/tinyxml.h"
#include "Common/Utility/kpuMap.h"
#include "Common/Utility/kpuLinkedList.h"

class kpuXmlParser
{
public:
	kpuXmlParser(void);
	~kpuXmlParser(void);

	bool			LoadFile(const char* szFile);

	//Iterating
	bool			HasElement() { return m_pCurrentElement != 0; }
	bool			HasNextSibling() { return m_pCurrentElement->NextSiblingElement() != 0; }
	bool			HasChild() { return m_pCurrentElement->FirstChildElement() != 0; }
	bool			HasParent() { return m_plParentList->Last()->GetPointer() != 0; }

	void			FirstElement()	{ m_pCurrentElement = m_XmlDoc.FirstChildElement(); }
	void			NextSiblingElement() { m_pCurrentElement = m_pCurrentElement->NextSiblingElement(); }
	void			FirstChildElement();
	void			FirstChildElement(const char* szName);
	void			Parent();

	//Attribute accessors
	bool			HasAttribute(const char* pszName);
	float			GetAttributeAsFloat(const char* pszName);
	int				GetAttributeAsInt(const char* pszName);
	bool			GetAttributeAsBool(const char* pszName);
	const char*		GetAttribute(const char* pszName);

	//Node Value accessors
	float			GetValueAsFloat();
	int				GetValueAsInt();
	const char*		GetValue();

	//Get Child Node Value
	float			GetChildValueAsFloat();
	int				GetChildValueAsInt();
	const char*		GetChildValue();

private:
	bool						IsInt(const char* szData);
	TiXmlDocument				m_XmlDoc;
	TiXmlElement*				m_pCurrentElement;
	kpuLinkedList*				m_plParentList;
	
	kpuMap<char*, char*>*	m_pDefineMap;

};

inline void kpuXmlParser::Parent()
{
	m_pCurrentElement = (TiXmlElement*)m_plParentList->Last()->GetPointer();

	//get rid of end of list
	delete m_plParentList->Last();
}

inline void kpuXmlParser::FirstChildElement()
{ 
	m_plParentList->AddTail(m_pCurrentElement);
	m_pCurrentElement = m_pCurrentElement->FirstChildElement(); 
}

inline void kpuXmlParser::FirstChildElement(const char* szName)
{
	m_plParentList->AddTail(m_pCurrentElement);
	m_pCurrentElement = m_pCurrentElement->FirstChildElement(szName);
}

