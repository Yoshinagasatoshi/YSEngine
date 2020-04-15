#include "stdafx.h"
#include "GameOver.h"
#include "gameObject/ysGameObjectManager.h"
#include "Title.h"
#include "Fade.h"

GameOver::GameOver()
{
	m_skinModel.Init(L"Assets/modelData/busyo.cmo");


	//ゲームオーバーはゲームオーバーの処理だけ書くべき、本来は美しくない。
	m_animClip[anim_Lose].Load(L"Assets/animData/busyo_dead.tka");
	m_animClip[anim_Lose].SetLoopFlag(false);


	m_sprite2.Init(L"Assets/sprite/Lose.dds", 640.0f, 360.0f);
	Fade::Getinstance().StartFadeOut();
	
	//アニメーション先モデルとアニメーションの個数を設定。
	m_animeModel.Init(
		m_skinModel,
		m_animClip,
		animClip_num
	);
}

GameOver::~GameOver()
{
	g_goMgr.DeleteGOObject(this);
}

void GameOver::Update()
{
	CVector3 cameraPos = { 0.0f,100.0f,300.0f };

	m_animeModel.Play(anim_Lose, 0.1f);

	if (g_pad->IsTrigger(enButtonA)) {
		Fade::Getinstance().StartFadeIn();
	}
	if (!Fade::Getinstance().IsFade()) {
	g_goMgr.NewGameObject<Title>("Title");
	DeleteGO(this);
	}
	g_camera3D.SetTarget(CVector3::Zero());
	
	g_camera3D.SetPosition(cameraPos);

	m_animeModel.Update(1.0f / 30.0f);
	m_sprite2.Update(m_position, m_rot, m_scale, m_pivot);
	m_skinModel.UpdateWorldMatrix(CVector3::Zero(), m_rot, m_scale);
}

void GameOver::Draw()
{
	m_skinModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void GameOver::PostDraw()
{
	m_sprite2.Draw();
}

