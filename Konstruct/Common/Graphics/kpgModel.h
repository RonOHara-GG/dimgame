#pragma once
#include "Common/Utility/kpuFixedArray.h"
#include "Common/Utility/kpuMap.h"
#include "Common/Graphics/kpgRenderer.h"
#include "Common/Utility/kpuPhysicalObject.h"

class kpuLinkedList;
class kpgGeometry;
class kpgGeometryInstance;
class TiXmlElement;
class kpuXmlParser;

class kpgModel: public kpuPhysicalObject
{
	typedef enum _eVertexSemantic
	{
		eVS_Unknown,

		eVS_Position,
		eVS_Normal,
		eVS_TexCoord,
		eVS_Weight,

	} eVertexSemantic;

	typedef struct _sSource
	{
		u32						uID;
		eVertexSemantic			eSemantic;
		kpuFixedArray<float>	aFloats;
		kpuFixedArray<u32>		aHashes;

	} sSource;


	typedef struct _sAnimation
	{
		sSource*					pSource;
		u32							uJoint;
		kpuFixedArray<float>		aTimes;
		kpuFixedArray<kpuMatrix>	aMatricies;
	}sAnimationData;



public:
	kpgModel(void);
	virtual ~kpgModel(void);

	void Rotate(float fXRadians, float fYRadians, float fZRadians);
	

	bool Load(const char* szFileName);

	void Draw(kpgRenderer* pRenderer = kpgRenderer::GetInstance());

	int GetInstanceCount()									{ return m_aInstances.GetNumElements(); }
	kpgGeometryInstance* GetInstance(int iIndex)			{ return m_aInstances[iIndex]; }	
	void SetGeometryInstance(kpgGeometryInstance* pInst);
	void SetGeometryInstance(kpgGeometryInstance* pInst, const kpuMatrix& mLocalToWolrd);

	kpuVector	GetPosition();
	void		SetPosition(kpuVector& vPos);
	void		SetScale(float fScaleX, float fScaleY, float fScaleZ);

	void		SetName(u32 iNameHash)						{ m_iNameHash = iNameHash; }
	u32			GetName()									{ return m_iNameHash; }
	
	kpuMatrix	GetMatrix();
	void		SetMatrix(const kpuMatrix mMatrix);

private:
	kpgTexture* LoadImage(kpuXmlParser* pParser);
	void LoadGeometryLibrary(kpuXmlParser* pParser, kpgTexture* pTexture);
	void LoadVisualSceneLibrary(kpuXmlParser* pParser);
	kpgGeometry* LoadMesh(kpuXmlParser* pParser);
	sSource* LoadSource(kpuXmlParser* pParser);
	void LoadVertices(kpuXmlParser* pParser, kpuLinkedList& sources);
	int* LoadTriangles(kpuXmlParser* pParser, kpuLinkedList& sources, int& iOutIndexCount);
	int* LoadPolygons(kpuXmlParser* pParser, kpuLinkedList& sources, int& iOutIndexCount);
	kpgGeometryInstance* LoadInstance(kpuXmlParser* pParser);

	//void LoadAnimationLibrary(kpuXmlParser* pParser);
	void LoadLibraryControllers(kpuXmlParser *pParser, kpuFixedArray<u32>* paVertexBoneIndices, kpuFixedArray<kpuVector>*	paVertexWeights);
	void LoadBoneIndicesWeights(kpuFixedArray<u32>* paBoneIndices, kpuFixedArray<kpuVector>* paWeights, sSource* pWeightSource, kpuXmlParser* pParser, const char* pszIndexCounts);
	void LoadJoints(kpuXmlParser* pParser, kpuLinkedList* sources);
	


protected:
	u32									m_iNameHash;
	kpuFixedArray<kpgGeometry*>			m_aGeometries;
	kpuFixedArray<kpgGeometryInstance*>	m_aInstances;
	kpuMap<u32, int>*					m_pBoneIndicieMap;
	kpuFixedArray<kpuMatrix>			m_aBoneMatricies;

	
};
