#include "stdafx.h"
#include "GameOver.h"
#include "gameObject/ysGameObjectManager.h"
#include "Title.h"
#include "Fade.h"

GameOver::GameOver()
{
	m_sprite1.Init(L"Assets/sprite/Lose1.dds", 640.0f, 360.0f);
	m_sprite2.Init(L"Assets/sprite/Lose2.dds", 640.0f, 360.0f);
	Fade::Getinstance().StartFadeOut();
}

GameOver::~GameOver()
{
	g_goMgr.DeleteGOObject(this);
}

void GameOver::Update()
{
	m_sprite1.Update(m_position,m_rot,m_scale,m_pivot);
	if (g_pad->IsTrigger(enButtonA)) {
		Fade::Getinstance().StartFadeIn();
	}
	if (!Fade::Getinstance().IsFade()) {
	g_goMgr.NewGameObject<Title>("Title");
	DeleteGO(this);
	}
}

void GameOver::PostDraw()
{
	m_sprite1.Draw();
}