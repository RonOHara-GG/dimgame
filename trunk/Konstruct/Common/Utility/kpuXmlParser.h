#pragma once
#include "External/tinyxml/tinyxml.h"
#include "Common/Utility/kpuMap.h"
#include "Common/Utility/kpuLinkedList.h"
#include "Common/Utility/kpuFixedArray.h"

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

class kpuXmlParser
{
public:
	kpuXmlParser(void);
	~kpuXmlParser(void);

	bool			LoadFile(const char* szFile);

	//Iterating
	bool			HasElement() { return m_pCurrentElement != 0; }
	bool			HasNextSibling() { return m_pCurrentElement->NextSiblingElement() != 0; }
	bool			HasChild() { return m_pCurrentElement->FirstChildElement() != 0; }
	bool			HasParent() { return m_lParentList.Last()->GetPointer() != 0; }

	void			FirstElement()	{ m_pCurrentElement = m_XmlDoc.FirstChildElement(); }
	void			NextSiblingElement() { m_pCurrentElement = m_pCurrentElement->NextSiblingElement(); }
	void			FirstChildElement();
	void			FirstChildElement(const char* szName);
	void			Parent();

	//Attribute accessors
	bool			HasAttribute(const char* pszName);
	float			GetAttributeAsFloat(const char* pszName);
	int				GetAttributeAsInt(const char* pszName);
	bool			GetAttributeAsBool(const char* pszName);
	const char*		GetAttribute(const char* pszName);

	//Node Value accessors
	float			GetValueAsFloat();
	int				GetValueAsInt();
	const char*		GetValue();

	//Get Child Node Value
	float			GetChildValueAsFloat();
	int				GetChildValueAsInt();
	const char*		GetChildValue();

	sSource*		LoadSource();

private:
	bool						IsInt(const char* szData);
	TiXmlDocument				m_XmlDoc;
	TiXmlElement*				m_pCurrentElement;
	kpuLinkedList				m_lParentList;
	
	kpuMap<char*, char*>*	m_pDefineMap;

};

#define s_uHash_source					0x1c3aff76
#define s_uHash_float_array 			0x8fcf3b99
#define s_uHash_Name_array 				0x6f845664
#define s_uHash_controller 				0xbf67c809
#define s_uHash_library_controllers		0xcdb8c8f0
#define s_uHash_input					0x0fa94ab5
#define s_uHash_library_visual_scenes	0x22ad11cd
#define s_uHash_node 					0x7c9b46ab
#define s_uHash_visual_scene 			0x8deb6446
#define s_uHash_JOINT					0xd66a849
#define s_uHash_NODE					0x7c892e2b
#define s_uHash_matrix					0xd3e291a
#define s_uHash_INV_BIND_MATRIX 		0x1be47042
#define s_uHash_joints					0x735ee7c

inline void kpuXmlParser::Parent()
{
	m_pCurrentElement = (TiXmlElement*)m_lParentList.Last()->GetPointer();

	//get rid of end of list
	if( m_lParentList.Last() != &m_lParentList )
		delete m_lParentList.Last();
}

inline void kpuXmlParser::FirstChildElement()
{ 
	m_lParentList.AddTail(m_pCurrentElement);
	m_pCurrentElement = m_pCurrentElement->FirstChildElement(); 
}

inline void kpuXmlParser::FirstChildElement(const char* szName)
{
	m_lParentList.AddTail(m_pCurrentElement);
	m_pCurrentElement = m_pCurrentElement->FirstChildElement(szName);
}

