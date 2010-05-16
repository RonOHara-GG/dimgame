#pragma once
#include "Common/Utility/kpuFixedArray.h"
#include "Common/Graphics/kpgRenderer.h"
#include "Common/Utility/kpuPhysicalObject.h"

class kpuLinkedList;
class kpgGeometry;
class kpgGeometryInstance;
class TiXmlElement;

class kpgModel: public kpuPhysicalObject
{
	typedef enum _eVertexSemantic
	{
		eVS_Unknown,

		eVS_Position,
		eVS_Normal,
		eVS_TexCoord,
	} eVertexSemantic;

	typedef struct _sSource
	{
		u32						uID;
		eVertexSemantic			eSemantic;
		kpuFixedArray<float>	aFloats;
	} sSource;

public:
	kpgModel(void);
	virtual ~kpgModel(void);

	void RotateY(float fRadians);

	bool Load(const char* szFileName);

	void Draw(kpgRenderer* pRenderer = kpgRenderer::GetInstance());

	int GetInstanceCount()									{ return m_aInstances.GetNumElements(); }
	kpgGeometryInstance* GetInstance(int iIndex)			{ return m_aInstances[iIndex]; }	
	void SetGeometryInstance(kpgGeometryInstance* pInst);
	void SetGeometryInstance(kpgGeometryInstance* pInst, const kpuMatrix& mLocalToWolrd);

	kpuVector GetPosition();
	void	  SetPosition(kpuVector& vPos);

private:
	kpgTexture* LoadImage(TiXmlElement* pLibrary);
	void LoadGeometryLibrary(TiXmlElement* pLibrary, kpgTexture* pTexture);
	void LoadVisualSceneLibrary(TiXmlElement* pLibrary);
	kpgGeometry* LoadMesh(TiXmlElement* pMeshElement);
	sSource* LoadSource(TiXmlElement* pSourceElement);
	void LoadVertices(TiXmlElement* pVerticesElement, kpuLinkedList& sources);
	int* LoadTriangles(TiXmlElement* pTrianglesElement, kpuLinkedList& sources, int& iOutIndexCount);
	int* LoadPolygons(TiXmlElement* pTrianglesElement, kpuLinkedList& sources, int& iOutIndexCount);
	kpgGeometryInstance* LoadInstance(TiXmlElement* pInstElement);
	


protected:
	kpuFixedArray<kpgGeometry*>			m_aGeometries;
	kpuFixedArray<kpgGeometryInstance*>	m_aInstances;

	
};
