#pragma once

#include "External/tinyxml/tinyxml.h"
#include "Common/Utility/kpuVector.h"
#include "Common/Utility/kpuLinkedList.h"

class kpgRenderer;
class kpgTexture;
class kpgUIManager;
class kpuPhysicalObject;
class kpuXmlParser;
enum eInputEventType;

class kpgUIWindow
{
public:

	enum eWindowType
	{
		eWT_GenericWindow,
		eWT_Text,
		eWT_TextInput,
		eWT_Button,
		eWT_List,
		eWT_Slider,

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

	kpgUIWindow(kpgUIManager* pManager);
	~kpgUIWindow(void);

	static kpgUIWindow* Load(kpuXmlParser* pParser, kpgUIManager* pManager);
	virtual TiXmlElement* Save(TiXmlNode* pParent);
	virtual void Load(kpuXmlParser* pParser);

	void LoadDefaults();

	void AddChildWindow(kpgUIWindow* pChild, const kpgRenderer* pRenderer);

	virtual void Update();
	virtual void Draw(kpgRenderer* pRenderer, const kpRect& rParent);

	kpgUIWindow* HitTest(float fX, float fY, const kpRect& rParent, eHitLocation* pOutLocation);

	// Translate a point from pixel space to homogeneous space
	void TransformPoint(float& fX, float& fY, const kpgRenderer* pRenderer);

	void SetVisible(bool bVal) { m_bVisible = bVal; } 
	void SetContext(kpuPhysicalObject* pObj) { m_pContextObj = pObj; }

	kpgUIWindow* GetChild(u32 uHash);
	kpgUIWindow* GetUIParent() { return m_pParent; }

	void SetHeight(float fHeight) { m_fHeight = fHeight; }
	
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
	
	bool HasFrame() { return m_bHasFrame; }
	bool IsVisible() { return m_bVisible; }
	void SetOrientation(eWindowOrientation eWO) { m_eOrientation = eWO; }

	u32	 GetHashCode()	{ return m_uHash; }

	void Open(kpuPhysicalObject* pContext = 0);	

	//Events and targets
	virtual u32  ClickEvent(float fX, float fY);

	virtual u32	 MouseEnterEvent()		{ return m_uMouseEnterEvent; }
	virtual u32	 MouseEnterParam()			{ return m_uMouseEnterParam; }

	virtual u32	 MouseExitEvent()		{ return m_uMouseExitEvent; }
	virtual u32	 MouseExitParam()			{ return m_uMouseExitParam; }

	virtual u32 MouseDrag(const kpuVector &vDelta, const kpuVector &vPos);

	u32 HandleInputEvent(eInputEventType type, u32 button); //return unhandled event, 0 if events are handled

protected:
	void Destroy();
	void DrawFrame(kpgRenderer* pRenderer, const kpRect& rRect);
	virtual void CalculateRectangle(const kpRect& rParent);
	virtual void GetDataSource();

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

	//Events and their targets
	u32					m_uClickEvent;
	u32					m_uClickParam;

	u32					m_uMouseEnterEvent;
	u32					m_uMouseEnterParam;

	u32					m_uMouseExitEvent;
	u32					m_uMouseExitParam;

	u32					m_uDragEvent;
	u32					m_uDragParam;

	//window name
	u32					m_uHash;
	char*				m_pDataSource;
	char*				m_pData;

	kpgUIManager*		m_pUIManager;
	kpuPhysicalObject*	m_pContextObj;
	
};


#define CE_NEW_WINDOW	0x40ed18e7	//Move to a new window
#define CE_OPEN			0x7c8a4b57 //Set a window to visible
#define CE_CLOSE		0xcf88a3b //Set a window to invisible
#define CE_SET_INPUT	0x0 //Set the current text input
#define CE_SCROLL_UP	0xebd294f9
#define CE_SCROLL_DOWN	0x2ac2704c
#define CE_SCROLL		0xd0bb1494
#define CE_SELECT_CELL	0x73fb9385