#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "Fade.h"
#include "gameObject/ysGameObjectManager.h"
Title::Title()
{
	//m_sprite = new Sprite();
	m_sprite.Init(L"Assets/sprite/Title.dds",1280.0f,720.0f);
	//
	CVector3 CameraPos = { 0.0f,0.0f,-600.0f };
	g_camera2D.SetWidth(FRAME_BUFFER_W);
	g_camera2D.SetHeight(FRAME_BUFFER_H);
	g_camera2D.SetPosition({ CameraPos });
	g_camera2D.SetTarget(CVector3::Zero());
	g_camera2D.Update();
	Fade::Getinstance().StartFadeOut();
}

Title::~Title()
{
	//g_goMgr.DeleteGOObject(this);
	//m_sprite.~Sprite();
}

void Title::Update()
{
	if (g_pad->IsTrigger(enButtonA)) {
		if (!Fade::Getinstance().IsFade()) {
			Game* game = g_goMgr.NewGameObject<Game>("Game");
			g_goMgr.DeleteGOObject(this);
		}
		else {
			Fade::Getinstance().StartFadeIn();
		}
	}

}

void Title::Draw()
{
	m_sprite.Draw();
}