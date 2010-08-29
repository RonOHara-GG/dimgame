#pragma once
#include "Common/Utility/kpuFixedArray.h"
#include "Common/Utility/kpuMap.h"
#include "Common/Graphics/kpgRenderer.h"
#include "Common/Utility/kpuPhysicalObject.h"
#include "Common/Utility/kpuXmlParser.h"

class kpuLinkedList;
class kpgGeometry;
class kpgGeometryInstance;
class TiXmlElement;
class kpgShader;
class kpgAnimation;
class kpgAnimationInstance;

class kpgModel: public kpuPhysicalObject
{
	

	typedef struct _sController
	{
		u32							uID;
		u32							uGeometryID;
		kpuFixedArray<u32>			aBoneIndices;
		kpuFixedArray<kpuVector>	aVertexWeights;
	}sController;


public:
	kpgModel(void);
	kpgModel(kpgModel& pOrig);
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

	void		SetShader(const char* pszShaderFile);

	//animation controls
	void UpdateAnimations(float fDeltaTime);
	void PlayAnimation(u32 uName);

private:
	kpgTexture* LoadImage(kpuXmlParser* pParser);
	void LoadGeometryLibrary(kpuXmlParser* pParser, kpgTexture* pTexture);
	void LoadVisualSceneLibrary(kpuXmlParser* pParser);
	kpgGeometry* LoadMesh(kpuXmlParser* pParser);
	
	void LoadVertices(kpuXmlParser* pParser, kpuLinkedList& sources);
	int* LoadTriangles(kpuXmlParser* pParser, kpuLinkedList& sources, int& iOutIndexCount);
	int* LoadPolygons(kpuXmlParser* pParser, kpuLinkedList& sources, int& iOutIndexCount);
	kpgGeometryInstance* LoadInstance(kpuXmlParser* pParser);

	
	void LoadLibraryControllers(kpuXmlParser *pParser);
	void LoadBoneIndicesWeights(sController* pController, sSource* pWeightSource, kpuXmlParser* pParser, const char* pszIndexCounts);
	//void LoadJoints(kpuXmlParser* pParser, kpuLinkedList* sources);
	//void LoadBoneMatricies(kpuXmlParser* pParser, kpuMatrix& mParent);	


protected:
	u32									 m_iNameHash;
	kpuFixedArray<kpgGeometry*>			 m_aGeometries;
	kpuFixedArray<kpgGeometryInstance*>	 m_aInstances;
	kpgAnimationInstance*				 m_pAnimationInstance;
	
	kpuLinkedList*						 m_pControllerList;	
	kpgShader*							 m_pShader;

	//Not for runtime
	/*kpuFixedArray<kpuMatrix>*			 m_paInvBindMatricies;
	kpuMap<u32,int>*					 m_pBoneIndicieMap;*/
	//kpuFixedArray<kpuMatrix>			 m_aBoneMatricies;	
};
