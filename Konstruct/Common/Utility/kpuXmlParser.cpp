#include "StdAfx.h"
#include "kpuXmlParser.h"
#include "Common/Utility/kpuFileManager.h"



kpuXmlParser::kpuXmlParser(void)
{
	m_pDefineMap = new kpuMap<char*, char*>();
	
}

kpuXmlParser::~kpuXmlParser(void)
{
	delete m_pDefineMap;
}

bool kpuXmlParser::LoadFile(const char *szFile)
{
	char szFileName[2048];
	kpuFileManager::GetFullFilePath(szFile, szFileName, sizeof(szFileName));

	if( m_XmlDoc.LoadFile(szFileName) )
	{
		//if file loaded check for any defines
		TiXmlElement* pElement = m_XmlDoc.FirstChildElement("Define");

		//if no defines found at top level check next level down
		if( !pElement )
			pElement = m_XmlDoc.FirstChildElement()->FirstChildElement("Define");

		while( pElement )
		{
			char* szKey = (char*)pElement->Attribute("Name");
			char* szVal = (char*)pElement->Attribute("Value");

			m_pDefineMap->Add(szKey, szVal);

			pElement = pElement->NextSiblingElement("Define");
		}

		FirstElement();
		return true;
	}

	return false;
}

bool kpuXmlParser::HasAttribute(const char* pszName)
{
	return m_pCurrentElement->Attribute(pszName) != 0;
}

float kpuXmlParser::GetAttributeAsFloat(const char* pszName)
{
	return (float)atof(GetAttribute(pszName));
}

bool kpuXmlParser::GetAttributeAsBool(const char* pszName)
{
	return GetAttributeAsInt(pszName) != 0;
}

int kpuXmlParser::GetAttributeAsInt(const char *pszName)
{
	const char* szVal = GetAttribute(pszName);

	if( szVal )
	{
		if( IsInt(szVal) )
			return atoi(szVal);

		return StringHash(szVal);
	}

	return 0;
}

const char* kpuXmlParser::GetAttribute(const char *pszName)
{
	const char* szVal = m_pCurrentElement->Attribute(pszName);

	if( szVal )
	{
		//see if it is in the map
		char** szMapped = (*m_pDefineMap)[(char*)szVal];

		if( szMapped )
			return *szMapped;

		return szVal;
	}

	return 0;
}

const char* kpuXmlParser::GetValue()
{
	const char* szVal = m_pCurrentElement->Value();
	
	//see if it is in the map
	char** szMapped = (*m_pDefineMap)[(char*)szVal];

	if( szMapped )
		return *szMapped;

	return szVal;
}

float kpuXmlParser::GetValueAsFloat()
{
	return (float)atof(GetValue());	
}

int	kpuXmlParser::GetValueAsInt()
{
	const char* szVal = GetValue();

	if( IsInt(szVal) )
		return atoi(szVal);

	return StringHash(szVal);
}

const char* kpuXmlParser::GetChildValue()
{
	const char* szVal = m_pCurrentElement->FirstChild()->Value();
	
	//see if it is in the map
	char** szMapped = (*m_pDefineMap)[(char*)szVal];

	if( szMapped )
		return *szMapped;

	return szVal;
}

float kpuXmlParser::GetChildValueAsFloat()
{
	return (float)atof(GetChildValue());	
}

int	kpuXmlParser::GetChildValueAsInt()
{
	const char* szVal = GetChildValue();

	if( IsInt(szVal) )
		return atoi(szVal);

	return StringHash(szVal);
}

bool kpuXmlParser::IsInt(const char *szData)
{
	//check to make sure it already isn't an int
	int iLen = strlen(szData);

	for(int i = 0; i < iLen; i++)
	{
		if( szData[i] - '0' > 9 )
		{
			//check for negative
			if( i == 0 )
			{
				if( szData[i] == '-' )
					continue;
			}
			return false;
		}
	}

	return true;
}

sSource* kpuXmlParser::LoadSource()
{
	sSource* pSource = new sSource();

	// Get the ID
	const char* pszID = GetAttribute("id");
	if( !pszID )
	{
		delete pSource;
		return 0;
	}
	pSource->uID = StringHash(pszID);

	// Find the float array
	FirstChildElement();
	while( HasElement() )
	{
		u32 uName = (u32)GetValueAsInt();
		if( uName == s_uHash_float_array || uName == s_uHash_Name_array)
		{
			const char* pszCount = GetAttribute("count");
			if( !pszCount )
			{
				delete pSource;
				return 0;
			}
			int iCount = atoi(pszCount);

			if( uName == s_uHash_float_array )
				pSource->aFloats.SetSize(iCount);
			else
				pSource->aHashes.SetSize(iCount);

			char* pszArray = _strdup(GetChildValue());
			char* pDataPtr = pszArray;
			for( int i = 0; i < iCount; i++ )
			{
				char* pStart = pDataPtr;
				while( *pDataPtr && *pDataPtr != ' ' ) pDataPtr++;
				*pDataPtr = 0;

				if( uName == s_uHash_float_array )
					pSource->aFloats[i] = (float)atof(pStart);
				else
					pSource->aHashes[i] = StringHash(pStart);

				pDataPtr++;
			}
			free(pszArray);

			pSource->eSemantic = eVS_Unknown;

			break;
		}		

		NextSiblingElement();
	}

	Parent();

	return pSource;
}
