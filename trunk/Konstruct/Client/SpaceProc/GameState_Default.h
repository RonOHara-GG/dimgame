#pragma once
#include "GameState.h"
#include "SunAndMoon.h"

class kppBox;
class kppPlane;
class kppTerrain;
class kppTexture;
class kppHuman;
class kpgGeometryInstance;
class kpgLight;
class kpgShader;

class GameState_Default :
	public GameState
{
public:
	GameState_Default(void);
	virtual ~GameState_Default(void);

	virtual void Update();
	virtual void Draw();

protected:
	SunAndMoon	m_SunAndMoon;
	kpgLight*	m_pSunLight;
	kppBox*		m_pMyBox;
	kppPlane*	m_pMyPlane;
	kppTexture*	m_pMyFloorTexture;
	kppTerrain*	m_pMyTerrain;

	kpgGeometryInstance*	m_pBoxes[4];
	kpgGeometryInstance*	m_pGroundPlane;
	kpgGeometryInstance*	m_pTerrain;

	kppHuman*				m_pMainCharacter;
	kpgGeometryInstance*	m_pMainCharacterInst;
};
