#include "StdAfx.h"
#include "GameState_FrontEnd.h"
#include "Common\Input\kpiInputManager.h"
#include "Common\graphics\kpgUIManager.h"
#include "Common\graphics\kpgRenderer.h"

GameState_FrontEnd::GameState_FrontEnd(void)
{
	m_pUIManager = new kpgUIManager();
	m_pUIManager->LoadWindows("Assets/UI/FrontEnd/FrontEndUI.xml");
}

GameState_FrontEnd::~GameState_FrontEnd(void)
{
}

void GameState_FrontEnd::MouseUpdate(int X, int Y)
{
}

void GameState_FrontEnd::Update(float fDeltaTime)
{
}

void GameState_FrontEnd::Draw()
{
	m_pUIManager->Draw(kpgRenderer::GetInstance());
}

void GameState_FrontEnd::AddActor(Actor* pActor)
{
}

bool GameState_FrontEnd::HandleInputEvent(eInputEventType type, u32 button)
{
	m_pUIManager->HandleInputEvent(type, button);
	return false;
}