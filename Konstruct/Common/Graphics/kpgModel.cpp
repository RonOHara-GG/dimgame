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
#include "External/tinyxml/tinyxml.h"
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

kpgModel::kpgModel(void)
{
	
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
}

bool kpgModel::Load(const char* cszFileName)
{
	bool bRet = false;
	// Load the level list
	char szFileName[2048];
	kpuFileManager::GetFullFilePath(cszFileName, szFileName, sizeof(szFileName));

	//Texture
	kpgTexture* pTexture = 0;

	TiXmlDocument doc;
	if( doc.LoadFile(szFileName) )
	{
		// Find the COLLADA tag
		for( TiXmlElement* pChild = doc.FirstChildElement(); pChild != 0; pChild = pChild->NextSiblingElement() )
		{
			if( !_strnicmp(pChild->Value(), "COLLADA", 7) )
			{
				bRet = true;

				// Parse libraries
				for( TiXmlElement* pEChild = pChild->FirstChildElement(); pEChild != 0; pEChild = pEChild->NextSiblingElement() )
				{
					u32 uHash = StringHash(pEChild->Value());
					switch( uHash )
					{
						case s_uHash_library_images:
							 pTexture = LoadImage(pEChild);
							break;
						case s_uHash_library_geometries:
							LoadGeometryLibrary(pEChild, pTexture);
							break;
						case s_uHash_library_visual_scenes:
							LoadVisualSceneLibrary(pEChild);
							break;
						default:
							break;
					}
				}
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

	return bRet;
}

void kpgModel::LoadGeometryLibrary(TiXmlElement* pLibrary, kpgTexture* pTexture)
{
	kpuLinkedList lGeometries;
	int iGeometryCount = 0;

	// Go through all the child elements
	for( TiXmlElement* pEGeometry = pLibrary->FirstChildElement(); pEGeometry != 0; pEGeometry = pEGeometry->NextSiblingElement() )
	{
		// Make sure this is a geometry element
		u32 uHash = StringHash(pEGeometry->Value());
		if( uHash == s_uHash_geometry )
		{
			// Load the mesh data
			for( TiXmlElement* pMesh = pEGeometry->FirstChildElement(); pMesh != 0; pMesh = pMesh->NextSiblingElement() )
			{
				uHash = StringHash(pMesh->Value());
				switch( uHash )
				{					
					case s_uHash_mesh:
						{
							kpgGeometry* pGeometry = LoadMesh(pMesh);
							if( pGeometry )
							{
								//uHash = StringHash(pEGeometry->Attribute("id"));
								//static const u32 test = StringHash("pPlaneShape17");
								//if( uHash == test )
								//	pGeometry->SetName(pEGeometry->Attribute("id"));
								pGeometry->SetName(pEGeometry->Attribute("id"));
								pGeometry->SetTexture(pTexture);
								lGeometries.AddTail(pGeometry);
								iGeometryCount++;
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
						Printf("kpgModel::LoadGeometryLibrary Unsupported Mesh Type: %s\n", pMesh->Value());
						break;
				}
			}
		}
	}

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

void kpgModel::LoadVisualSceneLibrary(TiXmlElement* pLibrary)
{
	kpuLinkedList lInstances;
	int iInstanceCount = 0;
	for( TiXmlElement* pElement = pLibrary->FirstChildElement(); pElement != 0; pElement = pElement->NextSiblingElement() )
	{
		u32 uHash = StringHash(pElement->Value());
		if( uHash == s_uHash_visual_scene )
		{
			for( TiXmlElement* pChild = pElement->FirstChildElement(); pChild != 0; pChild = pChild->NextSiblingElement() )
			{
				uHash = StringHash(pChild->Value());
				if( uHash == s_uHash_node )
				{
					kpgGeometryInstance* pInstance = LoadInstance(pChild);
					if( pInstance )
					{
						lInstances.AddTail(pInstance);
						iInstanceCount++;
					}
				}
			}
		}
	}

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

kpgGeometry* kpgModel::LoadMesh(TiXmlElement* pMeshElement)
{
	kpgGeometry* pGeometry = 0;
	kpuLinkedList sources;
	int* pIndexArray = 0;
	int iIndexCount;

	for( TiXmlElement* pChild = pMeshElement->FirstChildElement(); pChild != 0; pChild = pChild->NextSiblingElement() )
	{
		const char* sz = pChild->Value();
		u32 uHash = StringHash(sz);
		switch( uHash )
		{
			case s_uHash_source:
				{
					sSource* pSource = LoadSource(pChild);
					if( pSource )
						sources.AddTail(pSource);
				}
				break;
			case s_uHash_vertices:
				LoadVertices(pChild, sources);
				break;
			case s_uHash_polygons:
				pIndexArray = LoadPolygons(pChild, sources, iIndexCount);
				break;
			case s_uHash_triangles:
				pIndexArray = LoadTriangles(pChild, sources, iIndexCount);
				break;
			default:
				Printf("kpgModel::LoadMesh Unsupported Mesh Element: %s\n", pChild->Value());
				break;
		}
	}

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
		pGeometry->CreateVertexBuffer(iVertCount, kpgRenderer::ePT_TriList, eVF_Position0 | eVF_Normal0 | eVF_TexCoord0, kpgRenderer::GetInstance());
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
			kpuVector vUV(pUVs->aFloats[iIndex + 0], pUVs->aFloats[iIndex + 1], 0.0f, 0.0f);		
			
			pVertexBuffer->SetPosition(i, vPos);
			pVertexBuffer->SetNormal(i, vNrm);
			pVertexBuffer->SetUV(i, vUV);
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

kpgModel::sSource* kpgModel::LoadSource(TiXmlElement* pSourceElement)
{
	sSource* pSource = new sSource();

	// Get the ID
	const char* pszID = pSourceElement->Attribute("id");
	if( !pszID )
	{
		delete pSource;
		return 0;
	}
	pSource->uID = StringHash(pszID);

	// Find the float array
	for( TiXmlElement* pChild = pSourceElement->FirstChildElement(); pChild != 0; pChild = pChild->NextSiblingElement() )
	{
		u32 uHash = StringHash(pChild->Value());
		if( uHash == s_uHash_float_array )
		{
			const char* pszCount = pChild->Attribute("count");
			if( !pszCount )
			{
				delete pSource;
				return 0;
			}
			int iCount = atoi(pszCount);
			pSource->aFloats.SetSize(iCount);

			char* pszFloats = _strdup(pChild->FirstChild()->Value());
			char* pFloatPtr = pszFloats;
			for( int i = 0; i < iCount; i++ )
			{
				char* pStart = pFloatPtr;
				while( *pFloatPtr && *pFloatPtr != ' ' ) pFloatPtr++;
				*pFloatPtr = 0;

				pSource->aFloats[i] = (float)atof(pStart);
				pFloatPtr++;
			}
			free(pszFloats);

			pSource->eSemantic = eVS_Unknown;

			break;
		}
	}

	return pSource;
}

void kpgModel::LoadVertices(TiXmlElement* pVerticesElement, kpuLinkedList& sources)
{	
	for( TiXmlElement* pInput = pVerticesElement->FirstChildElement(); pInput != 0; pInput = pInput->NextSiblingElement() )
	{
		u32 uHash = StringHash(pInput->Value());
		if( uHash == s_uHash_input )
		{
			const char* pszSource = pInput->Attribute("source");
			const char* ppSource = pszSource + 1;
			uHash = StringHash(ppSource);
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
				const char* pszSemantic = pInput->Attribute("semantic");
				uHash = StringHash(pszSemantic);
				switch( uHash )
				{
					case s_uHash_POSITION:
						pTheSource->eSemantic = eVS_Position;
						break;
					case s_uHash_NORMAL:
						pTheSource->eSemantic = eVS_Normal;
						break;
					default:
						Printf("kpgModel::LoadVertices Unsupported semantic: %s\n", pszSemantic);
						break;
				}
			}
			else
				Printf("kpgModel::LoadVertices unknown source: %s\n", pszSource);
		}		
	}
}

int* kpgModel::LoadTriangles(TiXmlElement* pTrianglesElement, kpuLinkedList& sources, int& iOutIndexCount)
{	
	int iVertexOffset = -1;
	int iTexCoordOffset = -1;
	int iNormCoordOffset = -1;
	int iIndicies = 0;
	iOutIndexCount = atoi(pTrianglesElement->Attribute("count")) * 3;
	int* pIndexArray = new int[iOutIndexCount];
	sSource* pTextureSource;
	sSource* pNormalSource = 0;
	for( TiXmlElement* pElement = pTrianglesElement->FirstChildElement(); pElement != 0; pElement = pElement->NextSiblingElement() )
	{
		u32 uHash = StringHash(pElement->Value());
		if( uHash == s_uHash_input )
		{
			const char* pszSemantic = pElement->Attribute("semantic");
			uHash = StringHash(pszSemantic);
			switch( uHash )
			{
				case s_uHash_VERTEX:
					iVertexOffset = atoi(pElement->Attribute("offset"));
					iIndicies++;
					break;
				case s_uHash_TEXCOORD:
					{
						iTexCoordOffset = atoi(pElement->Attribute("offset"));
						iIndicies++;
						u32 uTexCoordSourceID = StringHash(pElement->Attribute("source") + 1);
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
						iNormCoordOffset = atoi(pElement->Attribute("offset"));
						u32 uNormalSourceID = StringHash(pElement->Attribute("source") + 1);
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
					Printf("kpgModel::LoadTriangles Unsupported semantic: %s\n", pszSemantic);
					break;
			}
		}
		else if( pElement->Value()[0] == 'p' && pElement->Value()[1] == 0 )
		{
			kpuFixedArray<float> aTemp(pTextureSource->aFloats.GetNumElements());
			memcpy(&aTemp[0], &pTextureSource->aFloats[0], sizeof(float) * aTemp.GetNumElements());

			char* pszIndices = _strdup(pElement->FirstChild()->Value());
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
			Printf("kpgModel::LoadTriangles unknown element tag: %s\n", pElement->Value());
	}

	return pIndexArray;
}

int* kpgModel::LoadPolygons(TiXmlElement* pPolygonsElement, kpuLinkedList& sources, int& iOutIndexCount)
{
	int iVertexOffset = -1;
	int iTexCoordOffset = -1;
	int iNormalOffset = -1;
	iOutIndexCount = atoi(pPolygonsElement->Attribute("count")) * 3;
	int* pIndexArray = new int[iOutIndexCount];
	sSource* pTextureSource;
	sSource* pNormalSource;

	for( TiXmlElement* pElement = pPolygonsElement->FirstChildElement(); pElement != 0; pElement = pElement->NextSiblingElement() )
	{
		u32 uHash = StringHash(pElement->Value());
		if( uHash == s_uHash_input )
		{
			const char* pszSemantic = pElement->Attribute("semantic");
			uHash = StringHash(pszSemantic);
			switch( uHash )
			{
				case s_uHash_VERTEX:
					iVertexOffset = atoi(pElement->Attribute("offset"));
					break;
				case s_uHash_TEXCOORD:
					{
						iTexCoordOffset = atoi(pElement->Attribute("offset"));
						u32 uTexCoordSourceID = StringHash(pElement->Attribute("source") + 1);
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
						iNormalOffset = atoi(pElement->Attribute("offset"));
						u32 uNormalSourceID = StringHash(pElement->Attribute("source") + 1);
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
					Printf("kpgModel::LoadPolygons Unsupported semantic: %s\n", pszSemantic);
					break;
			}
		}
		else if( pElement->Value()[0] == 'p' && pElement->Value()[1] == 0 )
		{
			kpuFixedArray<float> aTemp(pTextureSource->aFloats.GetNumElements());
			memcpy(&aTemp[0], &pTextureSource->aFloats[0], sizeof(float) * aTemp.GetNumElements());			
			
			char* pszIndices = _strdup(pElement->FirstChild()->Value());
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
			Printf("kpgModel::LoadPolygons unknown element tag: %s\n", pElement->Value());
	}

	return pIndexArray;
}

kpgTexture*  kpgModel::LoadImage(TiXmlElement* pLibrary)
{
	// Go through all the child elements
	for( TiXmlElement* pEGeometry = pLibrary->FirstChildElement(); pEGeometry != 0; pEGeometry = pEGeometry->NextSiblingElement() )
	{
		// Make sure this is a geometry element
		u32 uHash = StringHash(pEGeometry->Value());
		if( uHash == s_uHash_image )
		{
			char* pszFilename = (char*)pEGeometry->FirstChildElement()->FirstChild()->Value();
			
			int iLength = strlen(pszFilename);

			for(int i = 0; i < iLength - 2; i++)
			{
				pszFilename[i] = pszFilename[i+2];
			}

			pszFilename[iLength - 1] = 0;
			pszFilename[iLength - 2] = 0;

			char assetPath[512];

			strcpy_s(assetPath, sizeof(assetPath), "Assets//");
			strcat_s(assetPath, sizeof(assetPath), pszFilename);

			kpgTexture* texture = new kpgTexture();
			texture->Load(assetPath);
			return texture;
		}
	}
	
	return 0;
}
kpgGeometryInstance* kpgModel::LoadInstance(TiXmlElement* pInstElement)
{
	kpgGeometryInstance* pInst = 0;
	kpuMatrix mInst;
	mInst.Identity();

	for( TiXmlElement* pElement = pInstElement->FirstChildElement(); pElement != 0; pElement = pElement->NextSiblingElement() )
	{
		u32 uHash = StringHash(pElement->Value());
		switch( uHash )
		{
			case s_uHash_translate:
				{
					char* pszTranslation = _strdup(pElement->FirstChild()->Value());
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
					char* pszRotation = _strdup(pElement->FirstChild()->Value());
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

					uHash = StringHash(pElement->Attribute("sid"));
					switch( uHash )
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
					uHash = StringHash(pElement->Attribute("url") + 1);
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
			default:
				Printf("kpgModel::LoadInstance Unsupported instance property: %s\n", pElement->Value());
				break;
		}
	}

	return pInst;
}

void kpgModel::Draw(kpgRenderer* pRenderer)
{
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

