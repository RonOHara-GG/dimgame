#pragma once
#include "External/tinyxml/tinyxml.h"
#include "Common/Utility/kpuMap.h"

class kpuXmlParser
{
public:
	kpuXmlParser(void);
	~kpuXmlParser(void);

	bool			LoadFile(const char* szFile);

	//Iterating
	bool			HasNextSibling() { return m_pCurrentElement->NextSiblingElement() != 0; }
	bool			HasChild() { return m_pCurrentElement->FirstChildElement() != 0; }
	bool			HasParent() { return m_pCurrentElement->Parent() != 0; }

	void			FirstElement()	{ m_pCurrentElement = m_XmlDoc.FirstChildElement(); }
	void			NextSiblingElement() { m_pCurrentElement = m_pCurrentElement->NextSiblingElement(); }
	void			FirstChildElement() { m_pCurrentElement = m_pCurrentElement->FirstChildElement(); }
	void			Parent() { m_pCurrentElement = (TiXmlElement*)m_pCurrentElement->Parent(); }

	//Attribute accessors
	float			GetAttributeAsFloat(const char* pszName);
	int				GetAttributeAsInt(const char* pszName);
	const char*		GetAttribute(const char* pszName);

	//Node Value accessors
	float			GetNodeValueAsFloat(const char* pszName);
	int				GetNodeValueAsInt(const char* pszName);
	const char*		GetNodeValue(const char* pszName);

private:
	TiXmlDocument				m_XmlDoc;
	TiXmlElement*				m_pCurrentElement;
	
	kpuMap<const char*, const char*>*	m_pDefineMap;

};
