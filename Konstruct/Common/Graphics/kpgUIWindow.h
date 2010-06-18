#pragma once

#include "External/Tinyxml/tinyxml.h"
#include "Common/Utility/kpuLinkedList.h"

class kpgRenderer;
class kpgTexture;

class kpgUIWindow
{
public:

	enum eWindowType
	{
		eWT_GenericWindow,
		eWT_Text,
		eWT_TextInput,
		eWT_Button,

		eWT_Last
	};

	enum eWindowOrientation
	{
		eWO_TopLeft,
		eWO_Center
	};

	enum eHitLocation
	{
		eHL_Outside,
		eHL_LeftBorder,
		eHL_RightBorder,
		eHL_TopBorder,
		eHL_BottomBorder,
		eHL_TopLeftCorner,
		eHL_TopRightCorner,
		eHL_BottomLeftCorner,
		eHL_BottomRightCorner,
		eHL_Background,
	};

	enum eClickEvent
	{
		eCE_NewWindow,
	};

	kpgUIWindow(void);
	~kpgUIWindow(void);

	static kpgUIWindow* Load(TiXmlNode* pNode);
	virtual TiXmlElement* Save(TiXmlNode* pParent);
	virtual void Load(TiXmlElement* pNode);

	void LoadDefaults();

	void AddChildWindow(kpgUIWindow* pChild, float fX, float fY, const kpgRenderer* pRenderer);

	virtual void Update();
	virtual void Draw(kpgRenderer* pRenderer, const kpRect& rParent);

	kpgUIWindow* HitTest(float fX, float fY, const kpRect& rParent, eHitLocation* pOutLocation);

	// Translate a point from pixel space to homogeneous space
	void TransformPoint(float& fX, float& fY, const kpgRenderer* pRenderer);
	
	// Sizing functions
	const kpRect& GetRect(const kpRect& rParent);
	float GetLeft() const;
	float GetRight() const;
	float GetTop() const;
	float GetBottom() const;
	void GrowLeft(float fDelta);
	void GrowRight(float fDelta);
	void GrowTop(float fDelta);
	void GrowBottom(float fDelta);
	void GetPosition(float& fX, float& fY);
	void SetPosition(float fX, float fY);
	void Move(float fDeltaX, float fDeltaY);

	u32	 GetHashCode()	{ return m_uHash; }

protected:
	void Destroy();
	void DrawFrame(kpgRenderer* pRenderer, const kpRect& rRect);
	virtual void CalculateRectangle(const kpRect& rParent);

protected:
	char*				m_szName;
	eWindowType			m_eType;
	kpgTexture*			m_pBackground;
	kpgTexture*			m_pBorder;
	kpgTexture*			m_pCorner;

	eWindowOrientation	m_eOrientation;
	float				m_fPosition[2];
	float				m_fWidth;
	float				m_fHeight;
	bool				m_bVisible;
	bool				m_bHasFrame;
	kpRect				m_rRect;

	kpuLinkedList		m_lChildren;
	kpgUIWindow*		m_pParent;

	eClickEvent			m_eClickEvent;
	u32					m_uTargetHash;
	u32					m_uHash;
};
