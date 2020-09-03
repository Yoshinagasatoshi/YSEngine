#include "stdafx.h"
#include "GameClear.h"
#include "gameObject/ysGameObjectManager.h"
#include "Title.h"
#include "Fade.h"
//#include "sound/SoundEngine.h"
//#include "sound/SoundSource.h"
#include "InGameSoundDirector.h"
const float Spritewid = 320.0f;
GameClear::GameClear()
{
	m_soundEngine.Init();
	//音SE素材
	//m_bgm.Init(L"Assets/sound/StageClear.wav");
	//m_bgm.Play(true);
	//m_bgm.SetVolume(0.5f);

	//SoundDirector::GetInstans().TitleBGM();

	m_skinModel.Init(L"Assets/modelData/busyo.cmo");
	m_skinModelStage.Init(L"Assets/modelData/result_stage.cmo");

	m_animClip[anim_Win].Load(L"Assets/animData/busyo_katidoki.tka");
	m_animClip[anim_Win].SetLoopFlag(false);

	m_sprite1.Init(L"Assets/sprite/Win.dds",1280.0f, 240.0f);
	//m_sprite2.Init(L"Assets/sprite/Win2.dds", 960.0f, 360.0f);
	Fade::Getinstance().StartFadeOut();

	//アニメーション先モデルとアニメーションの個数を設定。
	m_animeModel.Init(
		m_skinModel,
		m_animClip,
		animClip_num
	);
}

GameClear::~GameClear()
{
	g_goMgr.DeleteGOObject(this);
}

void GameClear::Update()
{
	CVector3 cameraPos = { 0.0f,200.0f,300.0f };
	CVector3 targetPos = { 0.0f,100.0f,0.0f };
	m_animeModel.Play(anim_Win, 0.1f);
	//m_sprite2.Update(m_position, m_rot, m_scale, m_pivot);


	if (g_pad->IsTrigger(enButtonA)) {
		m_isWaitFadeOut = true;
	}
	if (m_isWaitFadeOut) {
		Fade::Getinstance().StartFadeIn();
		if (!Fade::Getinstance().IsFade()) {
			g_goMgr.NewGameObject<Title>("Title");
			DeleteGO(this);
		}
	}

	g_camera3D.SetTarget(targetPos);

	g_camera3D.SetPosition(cameraPos);
	m_sprite1.Update(m_position, m_rot, m_scale, m_pivot);
	m_animeModel.Update(1.0f / 30.0f);
	m_skinModel.UpdateWorldMatrix(CVector3::Zero(), m_rot, m_scale);
	m_skinModelStage.UpdateWorldMatrix(CVector3::Zero(), m_rot, m_scale);
}

void GameClear::Draw()
{
	//ステージクリア用のモデル2つだけを映している
	m_skinModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
	m_skinModelStage.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void GameClear::PostDraw()
{
	m_sprite1.Draw();
	//m_sprite2.Draw();
}