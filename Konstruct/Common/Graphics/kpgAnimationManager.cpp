#include "StdAfx.h"
#include "kpgAnimationManager.h"
#include "Common/Utility/kpuXmlParser.h"
#include "Common/Graphics/kpgDAEFileConstants.h"

kpgAnimationManager::kpgAnimationManager(void)
{
}

kpgAnimationManager::~kpgAnimationManager(void)
{
}

void kpgAnimationManager::LoadAnimation(const char *szFile)
{
	kpuXmlParser* pParser = new kpuXmlParser();

	if( pParser->LoadFile(szFile) )
	{
		while( pParser->HasElement() )
		{
			if( !_strnicmp(pParser->GetValue(), "COLLADA", 7) )
			{
				pParser->FirstChildElement();				
				while( pParser->HasElement() )
				{
					switch((u32)pParser->GetValueAsInt())
					{
					case s_uHash_library_controllers:
						//create bone map
						LoadBoneIndicies(pParser);
						break;
					case s_uHash_library_animations:
						//load animations
						LoadAnimationLibrary(pParser);
						break;
					}

					pParser->NextSiblingElement();
				}
				pParser->Parent();

			}

			pParser->NextSiblingElement();
		}
		pParser->Parent();
	}

	delete pParser;
}

void kpgAnimationManager::LoadBoneIndicies(kpuXmlParser *pParser)
{
	//find the source with the name array
	pParser->FirstChildElement();

	//make sure it is the controllers
	if( (u32)pParser->GetValueAsInt() == s_uHash_controller )
	{
		pParser->FirstChildElement();
		pParser->FirstChildElement();

		bool bFound = false;
		while(pParser->HasElement() )
		{		
			if( (u32)pParser->GetValueAsInt() == s_uHash_source)
			{
				pParser->FirstChildElement();

				//make sure it is the name array
				if( (u32)pParser->GetValueAsInt() == s_uHash_Name_array )
				{
					//name array found create the map
					bFound = true;
					
					int iCount = pParser->GetAttributeAsInt("count");
					char* szJoints = _strdup(pParser->GetChildValue());
					char* szNext = szJoints;

					for(int i = 0; i < iCount; i++)
					{
						char* pStart = szNext;

						while(*szNext && *szNext != ' ') szNext++;
						*szNext = 0;

						m_pBoneIndicieMap->Add(StringHash(pStart), i);
						szNext++;
					}
				}	
				pParser->Parent();
			}

			pParser->NextSiblingElement();
		}

		pParser->Parent();

		pParser->Parent();
		pParser->Parent();
	}

	pParser->Parent();
}

void kpgAnimationManager::LoadAnimationLibrary(kpuXmlParser *pParser)
{





}