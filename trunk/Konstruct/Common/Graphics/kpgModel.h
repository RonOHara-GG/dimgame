#pragma once
#include "Common/Utility/kpuFixedArray.h"
#include "Common/Graphics/kpgRenderer.h"

class kpuLinkedList;
class kpgGeometry;
class kpgGeometryInstance;
class TiXmlElement;

class kpgModel
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
	~kpgModel(void);

	bool Load(const char* szFileName);

	void Draw(kpgRenderer* pRenderer = kpgRenderer::GetInstance());

	int GetInstanceCount()									{ return m_aInstances.GetNumElements(); }
	kpgGeometryInstance* GetInstance(int iIndex)			{ return m_aInstances[iIndex]; }

private:
	void LoadGeometryLibrary(TiXmlElement* pLibrary);
	void LoadVisualSceneLibrary(TiXmlElement* pLibrary);
	kpgGeometry* LoadMesh(TiXmlElement* pMeshElement);
	sSource* LoadSource(TiXmlElement* pSourceElement);
	void LoadVertices(TiXmlElement* pVerticesElement, kpuLinkedList& sources);
	int* LoadTriangles(TiXmlElement* pTrianglesElement, kpuLinkedList& sources, int& iOutIndexCount);
	kpgGeometryInstance* LoadInstance(TiXmlElement* pInstElement);

protected:
	kpuFixedArray<kpgGeometry*>			m_aGeometries;
	kpuFixedArray<kpgGeometryInstance*>	m_aInstances;
};
