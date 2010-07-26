#include "StdAfx.h"
#include "kpuXmlParser.h"
#include "Common/Utility/kpuFileManager.h"

kpuXmlParser::kpuXmlParser(void)
{
	m_pDefineMap = new kpuMap<const char*, const char*>();
}

kpuXmlParser::~kpuXmlParser(void)
{
}

bool kpuXmlParser::LoadFile(const char *szFile)
{
	char szFileName[2048];
	kpuFileManager::GetFullFilePath(szFile, szFileName, sizeof(szFileName));

	if( m_XmlDoc.LoadFile(szFileName) )
	{
		//if file loaded check for any defines
		TiXmlElement* pElement = m_XmlDoc.FirstChildElement("Define");

		while( pElement )
		{
			const char* szKey = pElement->Attribute("Name");
			const char* szVal = pElement->Attribute("Value");

			m_pDefineMap->Add(szKey, szVal);

			pElement = pElement->NextSiblingElement("Define");
		}

		FirstElement();
		return true;
	}

	return false;
}

float kpuXmlParser::GetAttributeAsFloat(const char* pszName)
{
	return (float)atof(GetAttribute(pszName));
}

int kpuXmlParser::GetAttributeAsInt(const char *pszName)
{
	return atoi(GetAttribute(pszName));
}

const char* kpuXmlParser::GetAttribute(const char *pszName)
{
	const char* szVal = m_pCurrentElement->Attribute(pszName);

	if( szVal )
	{
		//see if it is in the map
		const char* szMapped = *(*m_pDefineMap)[szVal];

		if( szMapped )
			return szMapped;

		return szVal;
	}

	return 0;
}

const char* kpuXmlParser::GetNodeValue(const char *pszName)
{
	TiXmlNode* pNode = m_pCurrentElement->FirstChild(pszName);

	if( pNode )
	{
		const char* szVal = pNode->Value();
		
		//see if it is in the map
		const char* szMapped = *(*m_pDefineMap)[szVal];

		if( szMapped )
			return szMapped;

		return szVal;
		
	}

	return 0;
}

float kpuXmlParser::GetNodeValueAsFloat(const char* pszName)
{
	return (float)atof(GetNodeValue(pszName));
}

int	kpuXmlParser::GetNodeValueAsInt(const char *pszName)
{
	return atoi(GetNodeValue(pszName));
}