#include "StdAfx.h"
#include "Common/Graphics/kpgModel.h"
#include "Common/Graphics/kpgGeometry.h"
#include "Common/Graphics/kpgGeometryInstance.h"
#include "Common/Graphics/kpgRenderer.h"
#include "Common/Graphics/kpgShader.h"
#include "Common/Graphics/kpgTexture.h"
#include "Common/Graphics/kpgVertexFormat.h"
#include "Common/Graphics/kpgVertexBuffer.h"
#include "Common/Graphics/kpgIndexBuffer.h"
#include "Common/Utility/kpuFileManager.h"
#include "Common/Utility/kpuLinkedList.h"
#include "Common/Utility/kpuXmlParser.h"
#include <d3dx9tex.h>

static const u32 s_uHash_library_images =		0x5a2a2cef;
static const u32 s_uHash_library_geometries =	0x9a15dacd;
static const u32 s_uHash_library_visual_scenes=	0x22ad11cd;
static const u32 s_uHash_image =				0xfa87ca8;
static const u32 s_uHash_geometry =				0xf3c45451;
static const u32 s_uHash_convex_mesh =			0xb0431924;
static const u32 s_uHash_mesh =					0x7c9a91b2;
static const u32 s_uHash_spline =				0x1c4803b0;
static const u32 s_uHash_source =				0x1c3aff76;
static const u32 s_uHash_vertices =				0xd31fda6a;
static const u32 s_uHash_triangles =			0x6cb8b6ce;
static const u32 s_uHash_polygons =				0x3db5dee0;
static const u32 s_uHash_float_array =			0x8fcf3b99;
static const u32 s_uHash_input =				0x0fa94ab5;
static const u32 s_uHash_POSITION =				0xd87309ba;
static const u32 s_uHash_NORMAL =				0xc3953cee;
static const u32 s_uHash_VERTEX =				0xd589ab43;
static const u32 s_uHash_TEXCOORD =				0x92ee91cd;
static const u32 s_uHash_extra =				0x0f667509;
static const u32 s_uHash_visual_scene =			0x8deb6446;
static const u32 s_uHash_node =					0x7c9b46ab;
static const u32 s_uHash_translate =			0x2396ec73;
static const u32 s_uHash_rotate =				0x19e50454;
static const u32 s_uHash_instance_geometry =	0x1c78d0c5;
static const u32 s_uHash_rotateX =				0x56858f2c;
static const u32 s_uHash_rotateY =				0x56858f2d;
static const u32 s_uHash_rotateZ =				0x56858f2e;

static const u32 s_uHash_library_animations =	0x751f88c;
static const u32 s_uHash_animation =			0x318ebea5;
static const u32 s_uHash_OUTPUT =				0xc65820b6;
static const u32 s_uHash_INPUT =				0xd542215;
static const u32 s_uHash_sampler =				0x8260ff59;
static const u32 s_uHash_Name_array =			0x6f845664;
static const u32 s_uHash_joints	=				0x735ee7c;
static const u32 s_uHash_vertex_weights =		0x8160d17d;
static const u32 s_uHash_vcount =				0x225e1744;
static const u32 s_uHash_v =					0x2b61b;
static const u32 s_uHash_INV_BIND_MATRIX =		0x1be47042;
static const u32 s_uHash_JOINT =				0xd66a849;
static const u32 s_uHash_skin =					0x7c9df43a;
static const u32 s_uHash_WEIGHT =				0xd7d9ad8d;
static const u32 s_uHash_controller =			0xbf67c809;
static const u32 s_uHash_library_controllers =  0xcdb8c8f0;
static const u32 s_uHash_instance_controller =  0xe72d317d;

kpgModel::kpgModel(void)
{
	m_pBoneIndicieMap = new kpuMap<u32, int>();
	m_pControllerList = 0;
	m_pShader = 0;
}

kpgModel::~kpgModel(void)
{
	for( int i = 0; i < m_aGeometries.GetNumElements(); i++ )
	{
		kpgGeometry* pGeometry = m_aGeometries[i];
		if( pGeometry )
			delete pGeometry;
	}

	for( int i = 0; i < m_aInstances.GetNumElements(); i++ )
	{
		kpgGeometryInstance* pInstance = m_aInstances[i];
		if( pInstance )
			delete pInstance;
	}

	delete m_pBoneIndicieMap;

	if( m_pShader )
		delete m_pShader;
}

bool kpgModel::Load(const char* cszFileName)
{
	//bool bRet = false;
	//// Load the level list
	//char szFileName[512];
	//kpuFileManager::GetFullFilePath(cszFileName, szFileName, sizeof(szFileName));

	////Set root directory to model directory
	//char szOldRoot[512];
	//memcpy(szOldRoot, kpuFileManager::GetRoot(), sizeof(szOldRoot));
	//char* pszRoot = kpuFileManager::GetDirectory(szFileName);
	//kpuFileManager::SetRootPath(pszRoot);	

	////Texture
	//kpgTexture* pTexture = 0;

	//TiXmlDocument doc;
	//if( doc.LoadFile(szFileName) )
	//{
	//	// Find the COLLADA tag
	//	for( TiXmlElement* pChild = doc.FirstChildElement(); pChild != 0; pChild = pChild->NextSiblingElement() )
	//	{
	//		if( !_strnicmp(pChild->Value(), "COLLADA", 7) )
	//		{
	//			bRet = true;

	//			// Parse libraries
	//			for( TiXmlElement* pEChild = pChild->FirstChildElement(); pEChild != 0; pEChild = pEChild->NextSiblingElement() )
	//			{
	//				u32 uHash = StringHash(pEChild->Value());
	//				switch( uHash )
	//				{
	//					case s_uHash_library_images:
	//						 pTexture = LoadImage(pEChild);
	//						break;
	//					case s_uHash_library_geometries:
	//						LoadGeometryLibrary(pEChild, pTexture);
	//						break;
	//					case s_uHash_library_visual_scenes:
	//						LoadVisualSceneLibrary(pEChild);
	//						break;
	//					default:
	//						break;
	//				}
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	DWORD dwErr = GetLastError();
	//	Printf("Failed to load file: %s\n", szFileName);
	//}

	//kpuVector vMin, vMax;

	//if( m_aInstances.GetNumElements() > 0 )
	//{

	//	//Create bounding volumes for this model
	//	kpgVertexBuffer* vb = m_aInstances[0]->GetGeometry()->GetVertexBuffer();
	//	vb->Lock();		
	//	
	//	vMax = vb->GetPosition(0);
	//	vMin = vMax;		

	//	for(int i = 0; i < m_aInstances.GetNumElements(); i++)
	//	{
	//		vb = m_aInstances[i]->GetGeometry()->GetVertexBuffer();
	//		vb->Lock();

	//		for(int j = 0; j < vb->GetVertexCount(); j++)
	//		{
	//			kpuVector vNext = vb->GetPosition(j);

	//			if( vNext.GetX() > vMax.GetX() )
	//				vMax.SetX(vNext.GetX());
	//			if( vNext.GetY() > vMax.GetY() )
	//				vMax.SetY(vNext.GetY());
	//			if( vNext.GetZ() > vMax.GetZ() )
	//				vMax.SetZ(vNext.GetZ());

	//			if( vNext.GetX() < vMin.GetX() )
	//				vMin.SetX(vNext.GetX());
	//			if( vNext.GetY() < vMin.GetY() )
	//				vMin.SetY(vNext.GetY());
	//			if( vNext.GetZ() < vMin.GetZ() )
	//				vMin.SetZ(vNext.GetZ());
	//		}
	//		vb->Unlock();
	//	}

	//}

	//vMax.SetW(1);
	//vMin.SetW(1);

	//Init(vMin, vMax);	

	////reset root directory
	//kpuFileManager::SetRootPath(szOldRoot);
	//free(pszRoot);

	//return bRet;

	bool bRet = false;
	// Load the level list
	char szFileName[512];
	kpuFileManager::GetFullFilePath(cszFileName, szFileName, sizeof(szFileName));

	//Set root directory to model directory
	char szOldRoot[512];
	memcpy(szOldRoot, kpuFileManager::GetRoot(), sizeof(szOldRoot));
	char* pszRoot = kpuFileManager::GetDirectory(szFileName);
	kpuFileManager::SetRootPath(pszRoot);	

	//Texture
	kpgTexture* pTexture = 0;
	m_pControllerList = new kpuLinkedList();
	

	kpuXmlParser* pParser = new kpuXmlParser();
	if( pParser->LoadFile(szFileName) )
	{
		// Find the COLLADA tag
		for( ;pParser->HasElement(); pParser->NextSiblingElement())
		{
			if( !_strnicmp(pParser->GetValue(), "COLLADA", 7) )
			{
				bRet = true;

				// Parse libraries
				pParser->FirstChildElement();
				while( pParser->HasElement() )					
				{
					switch( (u32)pParser->GetValueAsInt() )
					{
						case s_uHash_library_images:
							 pTexture = LoadImage(pParser);
							break;
						case s_uHash_library_geometries:
							LoadGeometryLibrary(pParser, pTexture);
							break;
						case s_uHash_library_visual_scenes:
							LoadVisualSceneLibrary(pParser);
							break;
						case s_uHash_library_controllers:
							LoadLibraryControllers(pParser);
							break;
						default:
							break;
					}

					pParser->NextSiblingElement();
				}

				pParser->Parent();
			}
		}
	}
	else
	{
		DWORD dwErr = GetLastError();
		Printf("Failed to load file: %s\n", szFileName);
	}

	kpuVector vMin, vMax;

	if( m_aInstances.GetNumElements() > 0 )
	{
		//Create bounding volumes for this model
		kpgVertexBuffer* vb = m_aInstances[0]->GetGeometry()->GetVertexBuffer();
		vb->Lock();	
		
		
		vMax = vb->GetPosition(0);
		vMin = vMax;		

		for(int i = 0; i < m_aInstances.GetNumElements(); i++)
		{
			vb = m_aInstances[i]->GetGeometry()->GetVertexBuffer();
			vb->Lock();

			for(int j = 0; j < vb->GetVertexCount(); j++)
			{
				kpuVector vNext = vb->GetPosition(j);

				if( vNext.GetX() > vMax.GetX() )
					vMax.SetX(vNext.GetX());
				if( vNext.GetY() > vMax.GetY() )
					vMax.SetY(vNext.GetY());
				if( vNext.GetZ() > vMax.GetZ() )
					vMax.SetZ(vNext.GetZ());

				if( vNext.GetX() < vMin.GetX() )
					vMin.SetX(vNext.GetX());
				if( vNext.GetY() < vMin.GetY() )
					vMin.SetY(vNext.GetY());
				if( vNext.GetZ() < vMin.GetZ() )
					vMin.SetZ(vNext.GetZ());
			}
			vb->Unlock();
		}

	}

	vMax.SetW(1);
	vMin.SetW(1);

	Init(vMin, vMax);	

	//reset root directory
	kpuFileManager::SetRootPath(szOldRoot);
	free(pszRoot);

	delete pParser;

	//clear out controllers
	kpuLinkedList* pNext = m_pControllerList->Next();
	while( pNext )
	{
		delete pNext;
		pNext = m_pControllerList->Next();
	}
	delete m_pControllerList;
	m_pControllerList = 0;

	return bRet;
}

void kpgModel::LoadGeometryLibrary(kpuXmlParser* pParser, kpgTexture* pTexture)
{
	kpuLinkedList lGeometries;
	int iGeometryCount = 0;

	pParser->FirstChildElement();
	// Go through all the child elements
	while( pParser->HasElement() )
	{
		// Make sure this is a geometry element
		if( (u32)pParser->GetValueAsInt() == s_uHash_geometry )
		{
			pParser->FirstChildElement();

			while( pParser->HasElement() )
			{
				// Load the mesh data			
				switch( (u32)pParser->GetValueAsInt() )
				{					
					case s_uHash_mesh:
						{
							kpgGeometry* pGeometry = LoadMesh(pParser);
							if( pGeometry )
							{
								//uHash = StringHash(pEGeometry->Attribute("id"));
								//static const u32 test = StringHash("pPlaneShape17");
								//if( uHash == test )
								//	pGeometry->SetName(pEGeometry->Attribute("id"));
								pParser->Parent();

								pGeometry->SetName(pParser->GetAttribute("id"));
								pGeometry->SetTexture(pTexture);
								lGeometries.AddTail(pGeometry);
								iGeometryCount++;

								pParser->FirstChildElement();
							}
						}
						break;
					case s_uHash_extra:
						// Not supported but I dont want messages about this tag
						break;
					case s_uHash_convex_mesh:
					case s_uHash_spline:
						// Not Supported Yet
					default:
						Printf("kpgModel::LoadGeometryLibrary Unsupported Mesh Type: %s\n", pParser->GetValue());
						break;
				}

				pParser->NextSiblingElement();
			}

			pParser->Parent();
		}

		pParser->NextSiblingElement();
	}

	pParser->Parent();

	// Convert linked list into fixed array for speed later
	m_aGeometries.SetSize(iGeometryCount);
	int iIndex = 0;
	while( lGeometries.Next() )
	{
		kpuLinkedList* pIter = lGeometries.Next();
		kpgGeometry* pGeometry = (kpgGeometry*)pIter->GetPointer();
		m_aGeometries[iIndex++] = pGeometry;
		delete pIter;
	}

	
}

void kpgModel::LoadVisualSceneLibrary(kpuXmlParser* pParser)
{
	kpuLinkedList lInstances;
	int iInstanceCount = 0;
	pParser->FirstChildElement();

	while( pParser->HasElement() )
	{
		if( (u32)pParser->GetValueAsInt() == s_uHash_visual_scene )
		{
			pParser->FirstChildElement();
			while( pParser->HasElement() )
			{
				if( (u32)pParser->GetValueAsInt() == s_uHash_node )
				{
					kpgGeometryInstance* pInstance = LoadInstance(pParser);
					if( pInstance )
					{
						lInstances.AddTail(pInstance);
						iInstanceCount++;
					}
				}

				pParser->NextSiblingElement();
			}

			pParser->Parent();
		}

		pParser->NextSiblingElement();
	}

	pParser->Parent();

	// Convert linked list into fixed array for speed later
	m_aInstances.SetSize(iInstanceCount);
	int iIndex = 0;
	while( lInstances.Next() )
	{
		kpuLinkedList* pIter = lInstances.Next();
		kpgGeometryInstance* pInstance = (kpgGeometryInstance*)pIter->GetPointer();
		m_aInstances[iIndex++] = pInstance;
		delete pIter;
	}
}

kpgGeometry* kpgModel::LoadMesh(kpuXmlParser* pParser)
{
	kpgGeometry* pGeometry = 0;
	kpuLinkedList sources;
	int* pIndexArray = 0;
	int iIndexCount;

	pParser->FirstChildElement();
	while( pParser->HasElement() )
	{
		switch( (u32)pParser->GetValueAsInt() )
		{
			case s_uHash_source:
				{
					sSource* pSource = LoadSource(pParser);
					if( pSource )
						sources.AddTail(pSource);
				}
				break;
			case s_uHash_vertices:
				LoadVertices(pParser, sources);
				break;
			case s_uHash_polygons:
				pIndexArray = LoadPolygons(pParser, sources, iIndexCount);
				break;
			case s_uHash_triangles:
				pIndexArray = LoadTriangles(pParser, sources, iIndexCount);
				break;
			default:
				Printf("kpgModel::LoadMesh Unsupported Mesh Element: %s\n", pParser->GetValue());
				break;
		}
		pParser->NextSiblingElement();
	}

	pParser->Parent();

	if( pIndexArray )
	{
		// Build the mesh
		sSource* pPositions;
		sSource* pNormals;
		sSource* pUVs;

		kpuLinkedList* pSNode = sources.Next();
		while( pSNode )
		{
			sSource* pSource = (sSource*)pSNode->GetPointer();
			if( pSource )
			{
				switch( pSource->eSemantic )
				{
					case eVS_Position:
						pPositions = pSource;
						break;
					case eVS_Normal:
						pNormals = pSource;
						break;
					case eVS_TexCoord:
						pUVs = pSource;
						break;
					default:
						break;
				}
			}
			pSNode = pSNode->Next();
		}

		int iVertCount = pPositions->aFloats.GetNumElements() / 3;
		pGeometry = new kpgGeometry();
		pGeometry->CreateVertexBuffer(iVertCount, kpgRenderer::ePT_TriList, eVF_Position0 | eVF_Normal0 | eVF_TexCoord0 | eVF_BoneIndex0 | eVF_SkinWeight0, kpgRenderer::GetInstance());
		pGeometry->CreateIndexBuffer(iIndexCount);

		kpgVertexBuffer* pVertexBuffer = pGeometry->GetVertexBuffer();

		m_aCollisionPrimatives.Add(new kpuBoundingBox(CalculateBoundingBox(pPositions->aFloats)));

		pVertexBuffer->Lock();		

		for( int i = 0; i < iVertCount; i++ )
		{
			int iIndex = i * 3;
			kpuVector vPos(pPositions->aFloats[iIndex + 0], pPositions->aFloats[iIndex + 1], pPositions->aFloats[iIndex + 2], 0.0f);
			kpuVector vNrm(pNormals->aFloats[iIndex + 0], pNormals->aFloats[iIndex + 1], pNormals->aFloats[iIndex + 2], 0.0f);
			iIndex = i * 2;
			kpuVector vUV(pUVs->aFloats[iIndex + 0], -pUVs->aFloats[iIndex + 1], 0.0f, 0.0f);		
			
			pVertexBuffer->SetPosition(i, vPos);
			pVertexBuffer->SetNormal(i, vNrm);
			pVertexBuffer->SetUV(i, vUV);
		}

		//get geometry id
		pParser->Parent();
		u32 uHash = pParser->GetAttributeAsInt("id");
		pParser->FirstChildElement();

		//assign bones
		kpuLinkedList* pNext = m_pControllerList->Next();
		while( pNext )
		{
			sController* pController = (sController*)pNext->GetPointer();
			if( pController->uGeometryID == uHash )
			{
				for(int i = 0; i < pController->aBoneIndices.GetNumElements(); i++)
				{
					pVertexBuffer->SetBoneIndex(i, pController->aBoneIndices[i]);
					pVertexBuffer->SetSkinWeight(i, pController->aVertexWeights[i]);
				}

				break;
			}
			pNext = pNext->Next();
		}

		pVertexBuffer->Unlock();

		kpgIndexBuffer* pIndexBuffer = pGeometry->GetIndexBuffer();
		pIndexBuffer->Lock();
		for( int i = 0; i < iIndexCount; i++ )
		{
			pIndexBuffer->SetIndex(i, pIndexArray[i]);
		}
		pIndexBuffer->Unlock();

		delete[] pIndexArray;
	}

	while( sources.Next() )
	{
		kpuLinkedList* pIter = sources.Next();
		sSource* pSource = (sSource*)pIter->GetPointer();
		delete pSource;
		delete pIter;
	}
	
	return pGeometry;
}

kpgModel::sSource* kpgModel::LoadSource(kpuXmlParser* pParser)
{
	sSource* pSource = new sSource();

	// Get the ID
	const char* pszID = pParser->GetAttribute("id");
	if( !pszID )
	{
		delete pSource;
		return 0;
	}
	pSource->uID = StringHash(pszID);

	// Find the float array
	pParser->FirstChildElement();
	while( pParser->HasElement() )
	{
		u32 uName = (u32)pParser->GetValueAsInt();
		if( uName == s_uHash_float_array || uName == s_uHash_Name_array)
		{
			const char* pszCount = pParser->GetAttribute("count");
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

			char* pszArray = _strdup(pParser->GetChildValue());
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

		pParser->NextSiblingElement();
	}

	pParser->Parent();

	return pSource;
}

void kpgModel::LoadVertices(kpuXmlParser* pParser, kpuLinkedList& sources)
{	
	pParser->FirstChildElement();
	while( pParser->HasElement() )
	{
		if( (u32)pParser->GetValueAsInt() == s_uHash_input )
		{
			const char* pszSource = pParser->GetAttribute("source");
			const char* ppSource = pszSource + 1;
			u32 uHash = StringHash(ppSource);
			kpuLinkedList* pSourceNode = sources.Next();
			sSource* pTheSource = 0;
			while( pSourceNode )
			{
				sSource* pSource = (sSource*)pSourceNode->GetPointer();
				if( pSource && pSource->uID == uHash )
				{
					pTheSource = pSource;
					break;
				}
				pSourceNode = pSourceNode->Next();
			}

			if( pTheSource )
			{
				switch( (u32)pParser->GetAttributeAsInt("semantic") )
				{
					case s_uHash_POSITION:
						pTheSource->eSemantic = eVS_Position;
						break;
					case s_uHash_NORMAL:
						pTheSource->eSemantic = eVS_Normal;
						break;
					default:
						Printf("kpgModel::LoadVertices Unsupported semantic: %s\n", pParser->GetAttribute("semantic") );
						break;
				}
			}
			else
				Printf("kpgModel::LoadVertices unknown source: %s\n", pszSource);
		}	

		pParser->NextSiblingElement();
	}

	pParser->Parent();
}

int* kpgModel::LoadTriangles(kpuXmlParser* pParser, kpuLinkedList& sources, int& iOutIndexCount)
{	
	int iVertexOffset = -1;
	int iTexCoordOffset = -1;
	int iNormCoordOffset = -1;
	int iIndicies = 0;
	iOutIndexCount = pParser->GetAttributeAsInt("count") * 3;
	int* pIndexArray = new int[iOutIndexCount];
	sSource* pTextureSource;
	sSource* pNormalSource = 0;
	pParser->FirstChildElement();
	while( pParser->HasElement() )
	{
		if( (u32)pParser->GetValueAsInt() == s_uHash_input )
		{
			switch( (u32)pParser->GetAttributeAsInt("semantic") )
			{
				case s_uHash_VERTEX:
					iVertexOffset = pParser->GetAttributeAsInt("offset");
					iIndicies++;
					break;
				case s_uHash_TEXCOORD:
					{
						iTexCoordOffset = pParser->GetAttributeAsInt("offset");
						iIndicies++;
						u32 uTexCoordSourceID = StringHash(pParser->GetAttribute("source") + 1);
						kpuLinkedList* pSourceNode = sources.Next();
						while( pSourceNode )
						{
							sSource* pSource = (sSource*)pSourceNode->GetPointer();
							if( pSource && pSource->uID == uTexCoordSourceID )
							{
								pSource->eSemantic = eVS_TexCoord;
								pTextureSource = pSource;
								break;
							}
							pSourceNode = pSourceNode->Next();
						}
						break;
					}	
				case s_uHash_NORMAL:
					{
						iIndicies++;
						iNormCoordOffset = pParser->GetAttributeAsInt("offset");
						u32 uNormalSourceID = StringHash(pParser->GetAttribute("source") + 1);
						kpuLinkedList* pSourceNode = sources.Next();
						while( pSourceNode )
						{
							sSource* pSource = (sSource*)pSourceNode->GetPointer();
							if( pSource && pSource->uID == uNormalSourceID )
							{
								pSource->eSemantic = eVS_Normal;
								pNormalSource = pSource;
								break;
							}
							pSourceNode = pSourceNode->Next();
						}
					
					break;
					}
				default:
					Printf("kpgModel::LoadTriangles Unsupported semantic: %s\n", pParser->GetAttribute("semantic"));
					break;
			}
		}
		else if( pParser->GetValue()[0] == 'p' && pParser->GetValue()[1] == 0 )
		{
			kpuFixedArray<float> aTemp(pTextureSource->aFloats.GetNumElements());
			memcpy(&aTemp[0], &pTextureSource->aFloats[0], sizeof(float) * aTemp.GetNumElements());

			char* pszIndices = _strdup(pParser->GetChildValue());
			char* pIndexPtr = pszIndices;
			for( int i = 0; i < iOutIndexCount; i++ )
			{
				int indices[3];
				for( int j = 0; j < iIndicies; j++ )
				{
					char* pStart = pIndexPtr;
					while( *pIndexPtr && *pIndexPtr != ' ' ) pIndexPtr++;
					*pIndexPtr = 0;

					indices[j] = atoi(pStart);		
					pIndexPtr++;

					
				}

				pIndexArray[i] = indices[iVertexOffset];
				pTextureSource->aFloats[(indices[iVertexOffset] * 2) + 0] = aTemp[(indices[iTexCoordOffset] * 2) + 0];
				pTextureSource->aFloats[(indices[iVertexOffset] * 2) + 1] = aTemp[(indices[iTexCoordOffset] * 2) + 1];
			}
			free(pszIndices);
		}
		else
			Printf("kpgModel::LoadTriangles unknown element tag: %s\n", pParser->GetValue());

		pParser->NextSiblingElement();
	}

	pParser->Parent();

	return pIndexArray;
}

int* kpgModel::LoadPolygons(kpuXmlParser* pParser, kpuLinkedList& sources, int& iOutIndexCount)
{
	int iVertexOffset = -1;
	int iTexCoordOffset = -1;
	int iNormalOffset = -1;
	iOutIndexCount = pParser->GetAttributeAsInt("count") * 3;
	int* pIndexArray = new int[iOutIndexCount];
	sSource* pTextureSource;
	sSource* pNormalSource;
	pParser->FirstChildElement();
	while( pParser->HasElement() )
	{
		if( (u32)pParser->GetValueAsInt() == s_uHash_input )
		{
			switch( (u32)pParser->GetAttributeAsInt("semantic") )
			{
				case s_uHash_VERTEX:
					iVertexOffset = pParser->GetAttributeAsInt("offset");
					break;
				case s_uHash_TEXCOORD:
					{
						iTexCoordOffset = pParser->GetAttributeAsInt("offset");
						u32 uTexCoordSourceID = StringHash(pParser->GetAttribute("source") + 1);
						kpuLinkedList* pSourceNode = sources.Next();
						while( pSourceNode )
						{
							sSource* pSource = (sSource*)pSourceNode->GetPointer();
							if( pSource && pSource->uID == uTexCoordSourceID )
							{
								pSource->eSemantic = eVS_TexCoord;
								pTextureSource = pSource;
								break;
							}
							pSourceNode = pSourceNode->Next();
						}
					}					
					break;
				case s_uHash_NORMAL:
					{
						iNormalOffset = pParser->GetAttributeAsInt("offset");
						u32 uNormalSourceID = StringHash(pParser->GetAttribute("source") + 1);
						kpuLinkedList* pSourceNode = sources.Next();
						while( pSourceNode )
						{
							sSource* pSource = (sSource*)pSourceNode->GetPointer();
							if( pSource && pSource->uID == uNormalSourceID )
							{
								pSource->eSemantic = eVS_Normal;
								pNormalSource = pSource;
								break;
							}
							pSourceNode = pSourceNode->Next();
						}

					}
					break;
				default:
					Printf("kpgModel::LoadPolygons Unsupported semantic: %s\n", pParser->GetAttribute("semantic"));
					break;
			}
		}
		else if( pParser->GetValue()[0] == 'p' && pParser->GetValue()[1] == 0 )
		{
			kpuFixedArray<float> aTemp(pTextureSource->aFloats.GetNumElements());
			memcpy(&aTemp[0], &pTextureSource->aFloats[0], sizeof(float) * aTemp.GetNumElements());			
			
			char* pszIndices = _strdup(pParser->GetChildValue());
			char* pIndexPtr = pszIndices;

			for( int i = 0; i < 3; i++ )
			{
				int indices[3];
				for( int j = 0; j < 3; j++ )
				{
					char* pStart = pIndexPtr;
					while( *pIndexPtr && *pIndexPtr != ' ' ) pIndexPtr++;
					*pIndexPtr = 0;

					indices[j] = atoi(pStart);		
					pIndexPtr++;
				}

				pIndexArray[i] = indices[iVertexOffset];
				pTextureSource->aFloats[(indices[iVertexOffset] * 2) + 0] = aTemp[(indices[iTexCoordOffset] * 2) + 0];
				pTextureSource->aFloats[(indices[iVertexOffset] * 2) + 1] = aTemp[(indices[iTexCoordOffset] * 2) + 1];
			}

			free(pszIndices);			
			
		}
		else
			Printf("kpgModel::LoadPolygons unknown element tag: %s\n", pParser->GetValue());

		pParser->NextSiblingElement();
	}
	pParser->Parent();

	return pIndexArray;
}

kpgTexture*  kpgModel::LoadImage(kpuXmlParser* pParser)
{
	kpgTexture* texture = 0;

	// Go through all the child elements
	pParser->FirstChildElement();
	while( pParser->HasElement() )
	{
		// Make sure this is a geometry element
		if( (u32)pParser->GetValueAsInt() == s_uHash_image )
		{
			pParser->FirstChildElement();
			char* pszFilename = (char*)pParser->GetChildValue();			

			texture = new kpgTexture();
			texture->Load(pszFilename);

			pParser->Parent();
		}
		pParser->NextSiblingElement();
	}

	pParser->Parent();
	
	return texture;
}
kpgGeometryInstance* kpgModel::LoadInstance(kpuXmlParser* pParser)
{
	kpgGeometryInstance* pInst = 0;
	kpuMatrix mInst;
	mInst.Identity();

	pParser->FirstChildElement();

	while( pParser->HasElement() )
	{
		switch( (u32)pParser->GetValueAsInt() )
		{
			case s_uHash_translate:
				{
					char* pszTranslation = _strdup(pParser->GetChildValue());
					char* pPtr = pszTranslation;
					kpuVector vTranslation;
					float* pVec = (float*)&vTranslation;
					for( int i = 0; i < 3; i++ )
					{
						char* pStart = pPtr;
						while( *pPtr && *pPtr != ' ' ) pPtr++;
						*pPtr = 0;

						pVec[i] = (float)atof(pStart);
						pPtr++;
					}
					free(pszTranslation);
					pVec[3] = 1.0f;
					mInst.SetD(vTranslation);
				}
				break;
			case s_uHash_rotate:
				{
					char* pszRotation = _strdup(pParser->GetChildValue());
					char* pPtr = pszRotation;
					kpuVector vRotation;
					float* pVec = (float*)&vRotation;
					for( int i = 0; i < 4; i++ )
					{
						char* pStart = pPtr;
						while( *pPtr && *pPtr != ' ' ) pPtr++;
						*pPtr = 0;

						pVec[i] = (float)atof(pStart);
						pPtr++;
					}
					free(pszRotation);
					pVec[3] *= DEG_TO_RAD;

					switch( (u32)pParser->GetAttributeAsInt("sid") )
					{
						case s_uHash_rotateX:
							mInst.SetA(vRotation);
							break;
						case s_uHash_rotateY:
							mInst.SetB(vRotation);
							break;
						case s_uHash_rotateZ:
							mInst.SetC(vRotation);
							break;
					}					
				}
				break;
			case s_uHash_instance_geometry:
				{
					u32 uHash = StringHash(pParser->GetAttribute("url") + 1);
					for( int i = 0; i < m_aGeometries.GetNumElements(); i++ )
					{
						if( m_aGeometries[i]->GetName() == uHash )
						{
							pInst = new kpgGeometryInstance(m_aGeometries[i]);
							{
								kpuMatrix mTemp = mInst;
								mInst.SetAxisRotation(mTemp.GetA().GetW(), mTemp.GetB().GetW(), mTemp.GetC().GetW());
								mInst.SetD(mTemp.GetD());
							}
							pInst->SetMatrix(mInst);
							break;
						}
					}
				}			
				break;
			case s_uHash_instance_controller:
				{
					u32 uHash = StringHash(pParser->GetAttribute("url") + 1);
					kpuLinkedList* pNext = m_pControllerList->Next();
					while( pNext )
					{
						sController* pController = (sController*)pNext->GetPointer();
						if( pController->uID == uHash )
						{
							for( int i = 0; i < m_aGeometries.GetNumElements(); i++ )
							{
								if( m_aGeometries[i]->GetName() == pController->uGeometryID )
								{
									pInst = new kpgGeometryInstance(m_aGeometries[i]);
									{
										kpuMatrix mTemp = mInst;
										mInst.SetAxisRotation(mTemp.GetA().GetW(), mTemp.GetB().GetW(), mTemp.GetC().GetW());
										mInst.SetD(mTemp.GetD());
									}
									pInst->SetMatrix(mInst);
									break;
								}
							}

							break;
						}
						pNext = pNext->Next();
					}

				}
				break;
			default:
				Printf("kpgModel::LoadInstance Unsupported instance property: %s\n", pParser->GetValue());
				break;
		}

		pParser->NextSiblingElement();
	}

	pParser->Parent();

	return pInst;
}

void kpgModel::Draw(kpgRenderer* pRenderer)
{
	if( m_pShader )
		m_pShader->SetSkinningMatricies(&m_aBoneMatricies);

	for( int i = 0; i < m_aInstances.GetNumElements(); i++ )
	{
		pRenderer->DrawInstancedGeometry(m_aInstances[i]);
	}
}

void kpgModel::Rotate(float fXRadians,float fYRadians,float fZRadians)
{
	for( int i = 0; i < m_aInstances.GetNumElements(); i++ )
	{
		m_aInstances[i]->SetRotation(fXRadians, fYRadians, fZRadians);
	}

}

kpuMatrix kpgModel::GetMatrix()
{
	return m_aInstances[0]->GetMatrix();
}

void kpgModel::SetMatrix(const kpuMatrix mMatrix)
{
	for( int i = 0; i < m_aInstances.GetNumElements(); i++ )
	{
		m_aInstances[i]->SetMatrix(mMatrix);
	}
}

kpuVector kpgModel::GetPosition()
{
	return m_aInstances[0]->GetMatrix().GetD();
}

void kpgModel::SetPosition(kpuVector& vPos)
{
	for(int i = 0; i < m_aInstances.GetNumElements(); i++)
	{
		m_aInstances[i]->SetPosition(vPos.GetX(), vPos.GetY(), vPos.GetZ());
	}
}

void kpgModel::SetScale(float fScaleX, float fScaleY, float fScaleZ)
{
	kpuMatrix mScale;
	mScale.SetScale(fScaleX, fScaleY, fScaleZ);
	for( int i = 0; i < m_aInstances.GetNumElements(); i++ )
	{
		kpuMatrix mScaled = mScale * m_aInstances[i]->GetMatrix();
		m_aInstances[i]->SetMatrix(mScaled);
	}
}

void kpgModel::SetGeometryInstance(kpgGeometryInstance* pInst)
{
	m_aInstances.SetSize(1);
	m_aInstances.Add(new kpgGeometryInstance(pInst->GetGeometry()));
}

void kpgModel::SetGeometryInstance(kpgGeometryInstance* pInst, const kpuMatrix& mLocalToWorld)
{
	m_aInstances.SetSize(1);
	m_aInstances.Add(new kpgGeometryInstance(pInst->GetGeometry(), mLocalToWorld));
}

//void kpgModel::LoadAnimationLibrary(kpuXmlParser *pParser)
//{	
//	kpuLinkedList sources;
//
//	pParser->FirstChildElement();
//
//	while( pParser->HasElement() )
//	{
//		//make sure it is an animation
//		if( (u32)pParser->GetValueAsInt() == s_uHash_animation )
//		{
//			pParser->FirstChildElement();
//
//			sAnimationData* pData = new sAnimationData();
//
//			while( pParser->HasElement() )
//			{
//				switch( (u32)pParser->GetValueAsInt() )
//				{
//				case s_uHash_source:
//					{
//						sSource* pSource = LoadSource(pParser);
//						if( pSource )
//						{							
//							pData->pSource = pSource;
//							sources.AddTail(pData);
//						}
//						break;
//					}
//				case s_uHash_sampler:						
//					pParser->FirstChildElement();
//
//					while( pParser->HasElement() )
//					{
//						//get the semantics
//						u32 uSemantic = pParser->GetAttributeAsInt("semantic");
//						u32 uSource = pParser->GetAttributeAsInt("source");
//					
//						if( pData->pSource->uID == uSource )
//						{
//							switch( uSemantic )
//							{
//							case s_uHash_input:
//								pData->pSource->eSemantic = eVS_Input;
//								break;
//							case s_uHash_output:
//								pData->pSource->eSemantic = eVS_Output;
//								break;
//							default:
//								Printf("kpgModel::LoadAnimationLibrary Unsupported semantic: %s\n", pParser->GetAttribute("semantic"));
//								break;
//							}
//
//							break;
//						}							
//						
//						pParser->NextSiblingElement();
//					}
//					pParser->Parent();
//					break;			
//				case s_uHash_channel:
//					{
//						char* szBone = _strdup(pParser->GetAttribute("target"));
//						char* attrib = strchr(szBone, '/');
//						attrib* = 0;
//						attrib++;
//
//						pData->uJoint = StringHash(szBone);						
//
//						free(szBone);
//					}						
//
//					break;
//				}
//				
//				pParser->NextSiblingElement();
//			}
//			pParser->Parent();			
//		}
//		pParser->NextSiblingElement();
//}
//
//	pParser->Parent();
//	//build the list of matricies and times for the animations
//	kpuFixedArray<kpuMatrix> aMatricies;
//	kpuFixedArray<float> aTimes;
//
//	kpuLinkedList* pNext = sources.Next();
//
//	while( pNext )
//	{
//		sAnimationData* pData = (sAnimationData*)pNext->GetPointer();
//
//		int count = pData->pSource->aFloats.GetNumElements();
//		count /= 16;
//
//		//create matricies
//		for(int i = 0; i < count; i++)
//		{
//			kpuVector v(pData->pSource->aFloats[j * i ], pData->pSource->aFloats[j * i + 1], pData->pSource->aFloats[j * i + 2], pData->pSource->aFloats[j * i + 3]);
//			kpuVector v1(pData->pSource->aFloats[j * i + 4], pData->pSource->aFloats[j * i + 5], pData->pSource->aFloats[j * i + 6], pData->pSource->aFloats[j * i + 7]);
//			kpuVector v2(pData->pSource->aFloats[j * i + 8], pData->pSource->aFloats[j * i + 9], pData->pSource->aFloats[j * i + 10], pData->pSource->aFloats[j * i + 11]);
//			kpuVector v3(pData->pSource->aFloats[j * i + 12], pData->pSource->aFloats[j * i + 13], pData->pSource->aFloats[j * i + 14], pData->pSource->aFloats[j * i + 15]);
//
//			pData->aMatricies.Add(kpuMatrix(v, v1, v2,v3);			
//		}
//
//		count = pData->
//
//
//
//		pNext = pNext->Next();
//	}
//
//
//
//
//
//
//}

void kpgModel::LoadLibraryControllers(kpuXmlParser *pParser)
{
	kpuLinkedList	sources;
	char*			pVertexInfluenceCount = 0;
	sSource*		pWeightSource = 0;	

	pParser->FirstChildElement();

	while( pParser->HasElement() )
	{
		//make sure it is a controler
		if( pParser->GetValueAsInt() == s_uHash_controller )
		{
			sController* pController = new sController();
			pController->uID = pParser->GetAttributeAsInt("id");

			pParser->FirstChildElement();

			if( pParser->GetValueAsInt() == s_uHash_skin )
			{				
				pController->uGeometryID = StringHash(pParser->GetAttribute("source") + 1);
				pParser->FirstChildElement();

				while( pParser->HasElement() )
				{
					switch( (u32)pParser->GetValueAsInt() )
					{
					case s_uHash_source:
						{
							sSource* pSource = LoadSource(pParser);
							sources.AddTail(pSource);
							break;
						}
					case s_uHash_joints:
						LoadJoints(pParser, &sources);
						break;
					case s_uHash_vertex_weights:
						{
							int iVCount = pParser->GetAttributeAsInt("count");
							pController->aBoneIndices.SetSize(iVCount);
							pController->aVertexWeights.SetSize(iVCount);

							pParser->FirstChildElement();
							while( pParser->HasElement() )
							{
								switch( (u32)pParser->GetValueAsInt() )
								{
								case s_uHash_input:
									//get the semantics
									if( (u32)pParser->GetAttributeAsInt("semantic") == s_uHash_WEIGHT )
									{
										kpuLinkedList* pNext = sources.Next();
										while(pNext)
										{
											sSource* source = (sSource*)pNext->GetPointer();
											if( source->uID = (u32)pParser->GetAttributeAsInt("source") )
											{	
												source->eSemantic = eVS_Weight;	
												pWeightSource = source;
												break;
											}
										}
									}
									break;
								case s_uHash_vcount:									
									//get the number of influences per vert
									pVertexInfluenceCount = (char*)pParser->GetChildValue();									
									break;
								case s_uHash_v:
									LoadBoneIndicesWeights(pController, pWeightSource, pParser, pVertexInfluenceCount);								
									break;
								}

								pParser->NextSiblingElement();
							}
							pParser->Parent();
							
							break;
						}
					}

					pParser->NextSiblingElement();
				}

				pParser->Parent();
			}
			pParser->Parent();

			m_pControllerList->AddTail(pController);
		}
		pParser->NextSiblingElement();
	}
	pParser->Parent();

	//clean up sources
	kpuLinkedList* pNext = sources.Next();
	while( pNext )
	{
		delete pNext;
		pNext = sources.Next();
	}
}

void kpgModel::LoadJoints(kpuXmlParser* pParser, kpuLinkedList* sources)
{
	pParser->FirstChildElement();
	while( pParser->HasElement() )
	{
		u32 uSemantic = (u32)pParser->GetAttributeAsInt("semantic");
		if( uSemantic == s_uHash_JOINT )
		{
			//Fill the bone matrix with the joints
			kpuLinkedList* pNext = sources->Next();
			while(pNext)
			{
				sSource* source = (sSource*)pNext->GetPointer();
				if( source->uID = (u32)pParser->GetAttributeAsInt("source") )
				{
					//found the bone list
					for(int i = 0; i < source->aHashes.GetNumElements(); i++)					
						m_pBoneIndicieMap->Add(source->aHashes[i], i);

					//delete this node its usefullness is gone
					delete source;
					kpuLinkedList* pTemp = pNext->Next();
					delete pNext;

					pNext = pTemp;
					break;
				}
				pNext = pNext->Next();
			}
		}
		else if( uSemantic = s_uHash_INV_BIND_MATRIX )
		{
			//create list of bone matricies
			kpuLinkedList* pNext = sources->Next();
			while(pNext)
			{
				sSource* source = (sSource*)pNext->GetPointer();
				if( source->uID = (u32)pParser->GetAttributeAsInt("source") )
				{
					m_aBoneMatricies.SetSize(source->aFloats.GetNumElements() / 16 );
					//found the bone matricies
					for(int i = 0; i < source->aFloats.GetNumElements(); i+= 16)
					{
						kpuVector v(source->aFloats[i], source->aFloats[i + 1], source->aFloats[i + 2], source->aFloats[i + 3]);
						kpuVector v1(source->aFloats[i + 4], source->aFloats[i + 5], source->aFloats[i + 6], source->aFloats[i + 7]);
						kpuVector v2(source->aFloats[i + 8], source->aFloats[i + 9], source->aFloats[i + 10], source->aFloats[i + 11]);
						kpuVector v3(source->aFloats[i + 12], source->aFloats[i + 13], source->aFloats[i + 14], source->aFloats[i + 15]);

						m_aBoneMatricies.Add(kpuMatrix(v,v1,v2,v3));
						//kpuMatrix m;
						//m.Identity();
						//m_aBoneMatricies.Add(m);
					}

					//delete this node its usefullness is gone
					delete source;
					kpuLinkedList* pTemp = pNext->Next();
					delete pNext;

					pNext = pTemp;
				}	
				pNext = pNext->Next();
			}
		}

		pParser->NextSiblingElement();
	}
	pParser->Parent();
}

void kpgModel::LoadBoneIndicesWeights(sController* pController, sSource* pWeightSource, kpuXmlParser* pParser, const char* pszIndexCounts)
{
	//get the bones indicies
	char* pszArray = _strdup((char*)pParser->GetChildValue());
	char* pDataPtr = pszArray;
	char* pCurrentVert = (char*)pszIndexCounts;
	float fWeights[4];
	u32 uBoneIndicies[4];
	for(int i = 0; i < pController->aBoneIndices.GetNumElements(); i++ )
	{
		//get the count for this vertex
		int iCount = atoi(pCurrentVert);
		while( *pCurrentVert && *pCurrentVert != ' ' ) pCurrentVert++;
		pCurrentVert++;
		
		uBoneIndicies[0] = uBoneIndicies[1] = uBoneIndicies[2] = uBoneIndicies[3] = 0;		
		fWeights[0] = fWeights[1] = fWeights[2] = fWeights[3] = 0.0f;
		for( int j = 0; j < iCount; j++ )
		{
			//Get bone index
			char* pStart = pDataPtr;
			while( *pDataPtr && *pDataPtr != ' ' ) pDataPtr++;
			*pDataPtr = 0;

			//make index zero based
			uBoneIndicies[j] = atoi(pStart) - 1;
			
			pDataPtr++;			
			
			pStart = pDataPtr;
			while( *pDataPtr && *pDataPtr != ' ' ) pDataPtr++;
			*pDataPtr = 0;

			//make zero based
			int iWeightIndex = atoi(pStart) - 1;
			fWeights[j] = pWeightSource->aFloats[iWeightIndex];

			pDataPtr++;
		}			

		pController->aBoneIndices.Add( (uBoneIndicies[0]<< 24) | (uBoneIndicies[1] << 16) | (uBoneIndicies[2] << 8) | uBoneIndicies[1] );
		pController->aVertexWeights.Add(kpuVector(fWeights[0], fWeights[1], fWeights[2], fWeights[3]));
	}
	free(pszArray);
}

void kpgModel::SetShader(const char *pszShaderFile)
{
	m_pShader = new kpgShader();
	m_pShader->LoadFromFile(kpgRenderer::GetInstance(),pszShaderFile);

	for( int i = 0; i < m_aInstances.GetNumElements(); i++ )
	{		
		m_aInstances[i]->GetGeometry()->SetShader(m_pShader);
	}
}