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
	kpuFixedArray<kpgAnimation::sBone*>* bones = new kpuFixedArray<kpgAnimation::sBone*>(MAX_BONES);
	
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
						LoadControllers(pParser);
						break;
					case s_uHash_library_animations:
						//load animations
						LoadAnimationLibrary(pParser, *bones);
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
	CreateAnimation(uName, bones);

	//clean up data structures for loading animations
	if(	m_plSkeletonSources )
	{
		kpuLinkedList* pNext = m_plSkeletonSources->Next();
		while( pNext )
		{
			delete pNext->GetPointer();
			delete pNext;
			pNext = m_plSkeletonSources->Next();
		}
	
		delete m_plSkeletonSources;
		m_plSkeletonSources = 0;
	}

	
	if( m_plBoneHierarchy )
	{
		kpuLinkedList* pNext = m_plBoneHierarchy->Next();
		while( pNext )
		{
			delete pNext->GetPointer();
			delete pNext;
			pNext = m_plBoneHierarchy->Next();
		}		
		delete m_plBoneHierarchy;
		m_plBoneHierarchy = 0;
	}

	if(  m_paInvBindMatricies )
	{
		delete  m_paInvBindMatricies;
		 m_paInvBindMatricies = 0;
	}

	delete m_pBoneIndicieMap;
	m_pBoneIndicieMap = 0;
	delete pParser;
}

void kpgAnimationManager::CreateAnimation(u32 uName, kpuFixedArray<kpgAnimation::sBone*>* bones)
{
	//Make sure their are bones being animated, if so then create a new animation and add them to it
	if( m_paInvBindMatricies->GetNumElementsUsed() > 0 )
	{
		kpgAnimation* pAnimation = new kpgAnimation(m_paInvBindMatricies->GetNumElementsUsed());
		for(int i = 0; i < m_paInvBindMatricies->GetNumElementsUsed(); i++ )
		{
			kpgAnimation::sBone* pBone = (*bones)[i];
			if( pBone )
			{
				sBoneData* sData = (*m_pBoneIndicieMap)[pBone->uName];
				sBoneData* sParent = (*m_pBoneIndicieMap)[sData->uParent];				

				pBone->mInvBind = (*m_paInvBindMatricies)[i];
				pBone->mBindPose = sData->mWorld;
				kpuMatrix mParent, mLocalInv;
				mParent.Identity();
				if( sParent )
				{					
					mParent = sParent->mWorld;
					sData->mWorld = sData->mWorld * sParent->mWorld;					
					pBone->iParent = sParent->iIndex;	
				}		
				else				
					pBone->iParent = -1;

				if( pBone->aTransforms.GetNumElements() == 0 )
				{
					pBone->aTransforms.SetSize(1);
					/*kpuMatrix mInvBind = (*m_paInvBindMatricies)[sParent->iIndex];
					mInvBind.Invert();
					mInvBind = mInvBind * pBone->mInvBind;*/
					kpuMatrix mFinal = pBone->mBindPose;
					pBone->aTransforms.Add(mFinal);					
				}				

				for(int j = 0; j < pBone->aTransforms.GetNumElementsUsed(); j++)
				{					
					kpuMatrix mTransform = pBone->aTransforms[j];
					kpuMatrix mFinal = pBone->mInvBind * ( mTransform * mParent );
					pBone->aTransforms[j] = mFinal;
				}	

				

				pAnimation->AddBone(sData->iIndex, pBone);
			}
		}	
		m_pAnimations->Add(uName, pAnimation);
	}
			

	delete bones;
}

void kpgAnimationManager::LoadControllers(kpuXmlParser *pParser)
{
	m_plSkeletonSources = new kpuLinkedList();
	 m_paInvBindMatricies = new kpuFixedArray<kpuMatrix>();
	kpuLinkedList sources;

	pParser->FirstChildElement();

	//make sure it is the controllers
	if( (u32)pParser->GetValueAsInt() == s_uHash_controller )
	{
		pParser->FirstChildElement();
		pParser->FirstChildElement();

		while(pParser->HasElement() )
		{			
			switch( (u32)pParser->GetValueAsInt() )
			{
			case s_uHash_source:
				{
					sSource* pSource = pParser->LoadSource();
					sources.AddTail(pSource);
					break;
				}
			case s_uHash_joints:
				//load the skeleton and inverse bind matrices
				pParser->FirstChildElement();
				while( pParser->HasElement() )
				{
					switch( pParser->GetAttributeAsInt("semantic") )
					{
					case s_uHash_JOINT:
						LoadBoneIndicies(pParser, &sources);
						break;
					case s_uHash_INV_BIND_MATRIX:
						LoadInverseBinds(pParser, &sources);
						break;
					}
					pParser->NextSiblingElement();
				}
				pParser->Parent();
				break;
			}		

			pParser->NextSiblingElement();
		}

		pParser->Parent();
		pParser->Parent();
	}

	pParser->Parent();

	//delete all unused sources
	kpuLinkedList* pNext = sources.Next();
	while(pNext)
	{
		sSource* source = (sSource*)pNext->GetPointer();
		delete source;
		delete pNext;
		pNext = sources.Next();
	}		
}

void kpgAnimationManager::LoadBoneIndicies(kpuXmlParser* pParser, kpuLinkedList* pSources)
{	
	u32 uSourceID = StringHash(pParser->GetAttribute("source") + 1);

	//find the source that contains the name of the joints
	kpuLinkedList* pNext = pSources->Next();
	while( pNext )
	{
		sSource* source = (sSource*)pNext->GetPointer();
		if( uSourceID == source->uID )
		{
			//found joint array add to skeleton source list and remove from all sources
			m_plSkeletonSources->AddTail(pNext->GetPointer());
			delete pNext;
			break;
		}
		pNext = pNext->Next();
	}
}

void kpgAnimationManager::LoadInverseBinds(kpuXmlParser* pParser, kpuLinkedList* pSources)
{
	u32 uSourceID = StringHash(pParser->GetAttribute("source") + 1);

	//create array of skinning matricies
	kpuLinkedList* pNext = pSources->Next();
	while(pNext)
	{
		sSource* source = (sSource*)pNext->GetPointer();
		if( source->uID == uSourceID )
		{
			 m_paInvBindMatricies->SetSize(source->aFloats.GetNumElements() / 16 );
			//found the bone matricies
			for(int i = 0; i < source->aFloats.GetNumElements(); i+= 16)
			{
				kpuVector v(source->aFloats[i], source->aFloats[i + 1], source->aFloats[i + 2], source->aFloats[i + 3]);
				kpuVector v1(source->aFloats[i + 4], source->aFloats[i + 5], source->aFloats[i + 6], source->aFloats[i + 7]);
				kpuVector v2(source->aFloats[i + 8], source->aFloats[i + 9], source->aFloats[i + 10], source->aFloats[i + 11]);
				kpuVector v3(source->aFloats[i + 12], source->aFloats[i + 13], source->aFloats[i + 14], source->aFloats[i + 15]);

				kpuMatrix mMatrix(v,v1,v2,v3);
				//transpose for cpu
				mMatrix.Transpose();
				m_paInvBindMatricies->Add(mMatrix);
			}

			//delete this node its usefullness is gone
			delete source;
			kpuLinkedList* pTemp = pNext->Next();
			delete pNext;
			break;

			pNext = pTemp;
		}	
		pNext = pNext->Next();
	}
}

void kpgAnimationManager::LoadAnimationLibrary(kpuXmlParser *pParser, kpuFixedArray<kpgAnimation::sBone*>& bones)
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
									//find the bones
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
						bool bValid = false;

						//make sure this is a transform
						if( uMatrix == s_uHash_transform)
						{
							//If we still don't know which skeleton look for the skeleton with this bone
							if( !bSkeletonFound )								
								bSkeletonFound = CreateSkeletonMap(uTarget);

							//if skeleton found make sure this bone is in it
							if( bSkeletonFound )
							{
								sBoneData* pData = (*m_pBoneIndicieMap)[uTarget];
								if( pData )
								{
									bValid = true;
									pTransforms->uID = uTarget;	
								}
							}
						}

						//not a valid bone get rid of it
						if( !bValid )
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

			//if we have transformations for this bone create it
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

					for(int i = 0; i < pTransforms->aFloats.GetNumElements(); i+= 16)
					{
						kpuVector v(pTransforms->aFloats[i], pTransforms->aFloats[i + 1], pTransforms->aFloats[i + 2], pTransforms->aFloats[i + 3]);
						kpuVector v1(pTransforms->aFloats[i + 4], pTransforms->aFloats[i + 5], pTransforms->aFloats[i + 6], pTransforms->aFloats[i + 7]);
						kpuVector v2(pTransforms->aFloats[i + 8], pTransforms->aFloats[i + 9], pTransforms->aFloats[i + 10], pTransforms->aFloats[i + 11]);
						kpuVector v3(pTransforms->aFloats[i + 12], pTransforms->aFloats[i + 13], pTransforms->aFloats[i + 14], pTransforms->aFloats[i + 15]);
					
						kpuMatrix mMatrix(v, v1, v2, v3);
						//make matrix column major for cpu
						mMatrix.Transpose();
					
						int iKey = i / 16;						

						//see if this matrix is already contained
						if( iKey == 0 || pBone->aTransforms[pBone->aTransforms.GetNumElementsUsed() - 1] != mMatrix )
						{
							pBone->uName = pTransforms->uID;
							pBone->aTransforms.Add(mMatrix);
							pBone->aTimes.Add(pTimes->aFloats[iKey]);						
						}				

					}					
					bones[pData->iIndex] = pBone;		

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
	
		for(int i = 0; i < bones.GetNumElements(); i++)
		{
			if( bones[i] && bones[i]->uName == pData->uName )
			{
				bFound = true;				
				break;
			}
		}

		if( !bFound )
		{
			//make sure it is in map
			void* p = (*m_pBoneIndicieMap)[pData->uName];
			if( p )
			{
				kpgAnimation::sBone* pBone = new kpgAnimation::sBone();
				pBone->uName = pData->uName;			
				bones[pData->iIndex] = pBone;		
			}
		}

		delete pData;
		delete pNext;

		pNext = m_plBoneHierarchy->Next();
	}

}

bool kpgAnimationManager::CreateSkeletonMap(u32 uBone)
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
			//add bone hierarchy to map
			kpuLinkedList* pNext = m_plBoneHierarchy->Next();
			while( pNext )
			{
				//make sure the bone exist in the map
				sBoneData* pData = (sBoneData*)pNext->GetPointer();

				if( pData->uName == pSkeletonSource->aHashes[i] )	
				{
					//Set the world matrix for each bone
					char* pMatrix = _strdup(pData->pFloats);
					pData->mWorld = ParseMatrix(pMatrix);
					pData->mWorld.Transpose();
					free(pMatrix);

					pData->iIndex = i;

					m_pBoneIndicieMap->Add(pSkeletonSource->aHashes[i], *pData);						
				}				
				pNext =  pNext->Next();
			}
		}		
		return true;
	}
	return false;	
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
			if( pParser->GetValueAsInt() == s_uHash_matrix )			
				pBone->pFloats = (char*)pParser->GetChildValue();			
			else
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