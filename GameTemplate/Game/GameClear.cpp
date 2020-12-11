#include "stdafx.h"
#include "GameClear.h"
#include "gameObject/ysGameObjectManager.h"
#include "Title.h"
#include "Fade.h"
//#include "sound/SoundEngine.h"
//#include "sound/SoundSource.h"
#include "InGameSoundDirector.h"
const float SPRITE_H = 240.0f; //画像縦
const float halfVol = 0.5f;		//半分の音量、音量が大きいwaveファイルはここで調整
const float INTERPORATION_TIME_S = 0.1f;//補間時間・小、短めのアニメーション補間をしたいときに使う。 
const float FRAMERATE_30 = 1.0f / 30.0f;//フレームレート30　30fps
const float SLASH_POS_Y = -200.0f;//[百人切り達成]画像のY座標位置修正
const int ONE_HUNDRED = 100;		//百。単位

const CVector3 CAMERA_POS = { 0.0f,200.0f,300.0f };	//カメラ位置の調整
const CVector3 TARGET_POS = { 0.0f,100.0f,0.0f };	//カメラ注視点の調整
GameClear::GameClear()
{
	m_soundEngine.Init();
	//音SE素材
	m_bgm.Init(L"Assets/sound/StageClear.wav");
	m_bgm.Play(true);
	m_bgm.SetVolume(halfVol);

	m_skinModel.Init(L"Assets/modelData/busyo.cmo");
	m_skinModelStage.Init(L"Assets/modelData/result_stage.cmo");

	m_animClip[anim_Win].Load(L"Assets/animData/busyo_katidoki.tka");
	m_animClip[anim_Win].SetLoopFlag(false);

	m_sprite1.Init(L"Assets/sprite/Win.dds",FRAME_BUFFER_W, SPRITE_H);
	m_sprite2.Init(L"Assets/sprite/100_slashe.dds", 360.0f, 180.0f);
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
	m_bgm.Release();
	g_goMgr.DeleteGOObject(this);
}

void GameClear::Update()
{
	m_animeModel.Play(anim_Win, INTERPORATION_TIME_S);
	


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

	g_camera3D.SetTarget(TARGET_POS);

	g_camera3D.SetPosition(CAMERA_POS);
	m_sprite1.Update(m_position, m_rot, m_scale, m_pivot);
	//百人切り達成。のUI画像のポジションを設定
	CVector3 slashPos = m_position;
	slashPos.y += SLASH_POS_Y;
	m_sprite2.Update(slashPos, m_rot, m_scale, m_pivot);
	m_animeModel.Update(FRAMERATE_30);
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
	//100人以上倒していたら出す
	if (g_goMgr.GetCount() >= ONE_HUNDRED) {
		m_sprite2.Draw();
	}
}