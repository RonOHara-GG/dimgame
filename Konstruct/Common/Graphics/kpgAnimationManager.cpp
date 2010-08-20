#include "StdAfx.h"
#include "kpgAnimationManager.h"
#include "kpgAnimationInstance.h"
#include "Common/Utility/kpuXmlParser.h"
#include "kpgAnimation.h"
#include "Common/Utility/kpuMatrix.h"

static const u32 s_uHash_library_animations =	0x751f88c;
static const u32 s_uHash_animation =			0x318ebea5;
static const u32 s_uHash_OUTPUT =				0xc65820b6;
static const u32 s_uHash_INPUT =				0xd542215;
static const u32 s_uHash_sampler =				0x8260ff59;

static const u32 s_uHash_channel =				0xc237195e;
static const u32 s_uHash_transform =			0x2393dd81;

kpgAnimationManager* kpgAnimationManager::m_spAnimationManager = 0;

kpgAnimationManager::kpgAnimationManager(void)
{
	m_plSkeletonSources = 0;
	m_pAnimations = new kpuMap<u32, kpgAnimation*>(); 		
	m_pBoneIndicieMap = 0;		
	m_plBoneHierarchy = 0;
}

kpgAnimationManager::~kpgAnimationManager(void)
{
	if( m_plSkeletonSources )
		delete m_plSkeletonSources;
	if( m_pAnimations )
		delete m_pAnimations;
	if( m_pBoneIndicieMap )
		delete m_pBoneIndicieMap;
	if( m_plBoneHierarchy )
		delete m_plBoneHierarchy;
}

void kpgAnimationManager::LoadAnimation(const char *szFile, u32 uName)
{
	kpuXmlParser* pParser = new kpuXmlParser();
	kpuFixedArray<kpgAnimation::sBone*>* transforms = new kpuFixedArray<kpgAnimation::sBone*>(MAX_BONES);
	
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
						LoadAnimationLibrary(pParser, *transforms);
						break;
					case s_uHash_library_visual_scenes:
						//get skeleton tree
						LoadSkeleton(pParser);
					}

					pParser->NextSiblingElement();
				}
				pParser->Parent();

			}

			pParser->NextSiblingElement();
		}
		pParser->Parent();
	}

	//create animation
	if( transforms->GetNumElementsUsed() > 0 )
	{
		kpgAnimation* pAnimation = new kpgAnimation(transforms->GetNumElementsUsed());
		for(int i = 0; i < transforms->GetNumElementsUsed(); i++ )
		{
			kpgAnimation::sBone* pBone = (*transforms)[i];
			sBoneData* sData = (*m_pBoneIndicieMap)[pBone->uName];
			sBoneData* sParent = (*m_pBoneIndicieMap)[sData->uParent];

			if( sParent )
				pBone->iParent = sParent->iIndex;	
			else
				pBone->iParent = -1;

			pAnimation->AddBone(sData->iIndex, pBone);
		}			
		m_pAnimations->Add(uName, pAnimation);
	}

	delete m_pBoneIndicieMap;
	delete pParser;
}

void kpgAnimationManager::LoadBoneIndicies(kpuXmlParser *pParser)
{
	m_plSkeletonSources = new kpuLinkedList();
	//find the source with the name array
	pParser->FirstChildElement();

	//make sure it is the controllers
	if( (u32)pParser->GetValueAsInt() == s_uHash_controller )
	{
		pParser->FirstChildElement();
		pParser->FirstChildElement();

		while(pParser->HasElement() )
		{		
			if( (u32)pParser->GetValueAsInt() == s_uHash_source)
			{
				pParser->FirstChildElement();

				//make sure it is the name array
				if( (u32)pParser->GetValueAsInt() == s_uHash_Name_array )
				{
					pParser->Parent();

					sSource* pSource = pParser->LoadSource();
					m_plSkeletonSources->AddTail(pSource);

					pParser->FirstChildElement();
				
				}	
				pParser->Parent();
			}

			pParser->NextSiblingElement();
		}

		pParser->Parent();
		pParser->Parent();
	}

	pParser->Parent();
}

void kpgAnimationManager::LoadAnimationLibrary(kpuXmlParser *pParser, kpuFixedArray<kpgAnimation::sBone*>& transforms)
{
	bool bSkeletonFound = false;	

	pParser->FirstChildElement();

	while( pParser->HasElement() )
	{
		//make sure we have an animation
		if( (u32)pParser->GetValueAsInt() == s_uHash_animation )
		{
			kpuLinkedList sources;			
			sSource* pTransforms = 0;
			sSource* pTimes = 0;

			pParser->FirstChildElement();

			while( pParser->HasElement() )
			{
				switch( (u32)pParser->GetValueAsInt() )
				{
				case s_uHash_source:
					{
						sSource* pSource = pParser->LoadSource();
						if( pSource )
							sources.AddTail(pSource);
						break;
					}
				case s_uHash_sampler:
					pParser->FirstChildElement();

					while( pParser->HasElement() )
					{
						if( (u32)pParser->GetValueAsInt() == s_uHash_input )
						{
							switch( (u32)pParser->GetAttributeAsInt("semantic") )
							{
							case s_uHash_INPUT:
								{
									//find the times
									kpuLinkedList* pNext = sources.Next();

									while( pNext )
									{
										sSource* pSource = (sSource*)pNext->GetPointer();
										u32 uAttrib = StringHash(pParser->GetAttribute("source") + 1);

										if( pSource->uID == uAttrib )
										{											
											pTimes = pSource;
											delete pNext;
											break;
										}
										pNext = pNext->Next();
									}
								break;
								}
							case s_uHash_OUTPUT:
								{
									//find the transforms
									kpuLinkedList* pNext = sources.Next();

									while( pNext )
									{
										sSource* pSource = (sSource*)pNext->GetPointer();
										u32 uAttrib = StringHash(pParser->GetAttribute("source") + 1);

										if( pSource->uID == uAttrib )
										{
											pTransforms = pSource;
											delete pNext;
											break;
										}
										pNext = pNext->Next();
									}
									break;
								}

							}
						}
						pParser->NextSiblingElement();
					}
					pParser->Parent();				
					break;
				case s_uHash_channel:
					{
						char* pTarget = _strdup(pParser->GetAttribute("target"));
						char* pMatrix = strchr(pTarget, '/');
						*pMatrix = 0;
						pMatrix++;

						u32 uTarget = StringHash(pTarget);
						u32 uMatrix = StringHash(pMatrix);

						//make sure this is a transform
						if( uMatrix == s_uHash_transform )
						{
							//see if we still need to find the skeleton we are animating
							if( !bSkeletonFound )
							{
								CreateSkeletonMap(uTarget);
								assert(m_pBoneIndicieMap);
								bSkeletonFound = true;
							}

							pTransforms->uID = uTarget;					
						}
						else
						{
							delete pTransforms;
							pTransforms = 0;
							delete pTimes;
							pTimes = 0;
						}

						free(pTarget);
						break;
					}
				}
				pParser->NextSiblingElement();
			}

			pParser->Parent();

			
			if( pTransforms && pTimes )
			{
				//get bone data
				sBoneData* pData = (*m_pBoneIndicieMap)[pTransforms->uID];
				if( pData )
				{
					//Make matrices and clean up duplicates
					kpgAnimation::sBone* pBone = new kpgAnimation::sBone();
					pBone->aTransforms.SetSize(pTransforms->aFloats.GetNumElements() / 16);
					pBone->aTimes.SetSize(pTimes->aFloats.GetNumElements());
					//pBone->aTimes.Add(0.0f);

					////get transformation @ t = 0				
					//char* szMatrix = _strdup(pData->pFloats);
					//kpuMatrix matrix = ParseMatrix(szMatrix);			
					//pBone->aTransforms.Add(matrix);
					//free(szMatrix);
					
					for(int i = 0; i < pTransforms->aFloats.GetNumElements(); i+= 16)
					{
						kpuVector v(pTransforms->aFloats[i], pTransforms->aFloats[i + 1], pTransforms->aFloats[i + 2], pTransforms->aFloats[i + 3]);
						kpuVector v1(pTransforms->aFloats[i + 4], pTransforms->aFloats[i + 5], pTransforms->aFloats[i + 6], pTransforms->aFloats[i + 7]);
						kpuVector v2(pTransforms->aFloats[i + 8], pTransforms->aFloats[i + 9], pTransforms->aFloats[i + 10], pTransforms->aFloats[i + 11]);
						kpuVector v3(pTransforms->aFloats[i + 12], pTransforms->aFloats[i + 13], pTransforms->aFloats[i + 14], pTransforms->aFloats[i + 15]);
					
						kpuMatrix mMatrix(v, v1, v2, v3);

						//see if this matrix is already contained
						if( i > 0 && pBone->aTransforms[(i / 16) - 1] != mMatrix )
						{
							pBone->uName = pTransforms->uID;
							pBone->aTransforms.Add(mMatrix);
							pBone->aTimes.Add(pTimes->aFloats[i / 16]);						
						}				

					}					

					transforms.Add(pBone);		

					delete pTransforms;
					delete pTimes;
					pTransforms = 0;
					pTimes = 0;
				}
			}
		}

		pParser->NextSiblingElement();
	}
	pParser->Parent();

	//make sure all bones are in the array and clean up bone heirarchy
	kpuLinkedList* pNext = m_plBoneHierarchy->Next();
	while( pNext )
	{
		bool bFound = false;
		sBoneData* pData = (sBoneData*)pNext->GetPointer();

		for(int i = 0; i < transforms.GetNumElementsUsed(); i++)
		{
			if( transforms[i]->uName == pData->uName )
			{
				bFound = true;				
				break;
			}
		}

		if( !bFound )
		{
			kpgAnimation::sBone* pBone = new kpgAnimation::sBone();
			pBone->uName = pData->uName;
			//pBone->aTransforms.SetSize(1);
			//pBone->aTimes.SetSize(1);
			//pBone->aTimes.Add(0.0f);

			////get transformation @ t = 0			
			//char* szMatrix = _strdup(pData->pFloats);
			//kpuMatrix matrix = ParseMatrix(szMatrix);			
			//pBone->aTransforms.Add(matrix);
			//free(szMatrix);
			transforms.Add(pBone);			
		}

		delete pData;
		delete pNext;

		pNext = m_plBoneHierarchy->Next();
	}
	delete m_plBoneHierarchy;

}

void kpgAnimationManager::CreateSkeletonMap(u32 uBone)
{
	sSource* pSkeletonSource = 0;

	kpuLinkedList* pNext = m_plSkeletonSources->Next();
	while( pNext && !pSkeletonSource )
	{
		sSource* pSource = (sSource*)pNext->GetPointer();

		for(int i =0; i < pSource->aHashes.GetNumElements(); i++)
		{
			if( uBone == pSource->aHashes[i] )
			{				
				pSkeletonSource = pSource;
				break;
			}
		}

		pNext = pNext->Next();
	}

	if( pSkeletonSource )
	{
		if( m_pBoneIndicieMap )
			delete m_pBoneIndicieMap;

		m_pBoneIndicieMap = new kpuMap<u32, sBoneData>();

		//store it in map
		for(int i = 0; i < pSkeletonSource->aHashes.GetNumElements(); i++)
		{
			sBoneData data;
			data.iIndex = i;
			m_pBoneIndicieMap->Add(pSkeletonSource->aHashes[i], data);			
		}

		//add bone hierarchy to map
		kpuLinkedList* pNext = m_plBoneHierarchy->Next();
		while( pNext )
		{
			//make sure the bone exist in the map
			sBoneData* pData = (sBoneData*)pNext->GetPointer();
			sBoneData* pBone = (*m_pBoneIndicieMap)[pData->uName];

			kpuLinkedList* pTemp = pNext->Next();

			if( pBone )
			{
				pBone->uParent = pData->uParent;
				pBone->pFloats = pData->pFloats;
			}
			else
			{
				delete pData;
				delete pNext;
			}

			pNext = pTemp;
		}
	}

	//clean up skeleton sources
	pNext = m_plSkeletonSources->Next();
	while( pNext )
	{
		delete pNext->GetPointer();
		delete pNext;
		pNext = m_plSkeletonSources->Next();
	}
	
	delete m_plSkeletonSources;
}

kpgAnimationManager* kpgAnimationManager::GetInstance()
{
	if( !m_spAnimationManager )
		m_spAnimationManager = new kpgAnimationManager();
	return m_spAnimationManager;
}

void kpgAnimationManager::LoadSkeleton(kpuXmlParser* pParser)
{
	pParser->FirstChildElement();

	if( (u32)pParser->GetValueAsInt() == s_uHash_visual_scene )
	{
		m_plBoneHierarchy = new kpuLinkedList();

		pParser->FirstChildElement();
		while( pParser->HasElement() )
		{			
			//find nodes containing joints and create the skeleton
			LoadBone(pParser, 0);			

			pParser->NextSiblingElement();
		}
		pParser->Parent();
	}
	pParser->Parent();
}

void kpgAnimationManager::LoadBone(kpuXmlParser *pParser, u32 uParent)
{
	if( pParser->GetValueAsInt() == s_uHash_node && pParser->GetAttributeAsInt("type") == s_uHash_JOINT )
	{
		//found a bone
		sBoneData* pBone = new sBoneData();
		pBone->uParent = uParent;
		pBone->uName = (u32)pParser->GetAttributeAsInt("name");
		m_plBoneHierarchy->AddTail(pBone);

		pParser->FirstChildElement();
		
		while( pParser->HasElement() )
		{
			//add the bones transformation or its child bones
			/*if( (u32)pParser->GetValueAsInt() == s_uHash_matrix )
				pBone->pFloats = (char*)pParser->GetChildValue();
			else*/
				LoadBone(pParser, pBone->uName);
			

			pParser->NextSiblingElement();
		}
		pParser->Parent();		
	}
}

kpgAnimationInstance*	kpgAnimationManager::GetNewAnimation(u32 uName)
{
	kpgAnimation** pAnimation = (*m_pAnimations)[uName];

	if( pAnimation )	
		return new kpgAnimationInstance(*pAnimation);

	return 0;
}