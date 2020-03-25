#include "stdafx.h"
#include "GameClear.h"
#include "gameObject/ysGameObjectManager.h"
#include "Title.h"

GameClear::GameClear()
{
	m_sprite1.Init(L"Assets/sprite/Win1.dds", 640.0f, 360.0f);
	m_sprite2.Init(L"Assets/sprite/Win2.dds", 640.0f, 360.0f);
}

GameClear::~GameClear()
{
	g_goMgr.DeleteGOObject(this);
}

void GameClear::Update()
{
	m_sprite1.Update(m_position, m_rot, m_scale, m_pivot);
	if (g_pad->IsTrigger(enButtonA)) {
		g_goMgr.NewGameObject<Title>("Title");
		DeleteGO(this);
	}
}

void GameClear::PostDraw()
{
	m_sprite1.Draw();
	m_sprite2.Draw();
}