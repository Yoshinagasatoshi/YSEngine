#include "stdafx.h"
#include "GameOver.h"
#include "gameObject/ysGameObjectManager.h"
#include "Title.h"
#include "Fade.h"
#include "InGameSoundDirector.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

const float BGM_VOL = 0.2f;
const float SpriteH = 240.0f;

const CVector3 cameraPos = { 0.0f,100.0f,300.0f };
const float INTERPORATION_TIME_S = 0.1f;//補間時間・小、短めのアニメーション補間をしたいときに使う。 
const float FRAMERATE_30 = 1.0f / 30.0f;//フレームレート30　30fps
GameOver::GameOver()
{
	m_soundEngine.Init();
	//音SE素材
	m_bgm.Init(L"Assets/sound/GameOver.wav");
	m_bgm.Play(true);
	m_bgm.SetVolume(BGM_VOL);
	//SoundDirector::GetInstans().SoundInit();
	//SoundDirector::GetInstans().GameOverBGM();

	m_skinModel.Init(L"Assets/modelData/busyo.cmo");
	m_skinModelStage.Init(L"Assets/modelData/result_stage.cmo");

	//プレイヤーが倒れるアニメーションを読み込む
	m_animClip[anim_Lose].Load(L"Assets/animData/busyo_dead.tka");
	m_animClip[anim_Lose].SetLoopFlag(false);


	m_sprite2.Init(L"Assets/sprite/Lose.dds", FRAME_BUFFER_W, SpriteH);
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
	m_animeModel.Play(anim_Lose, INTERPORATION_TIME_S);

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
	g_camera3D.SetTarget(CVector3::Zero());
	
	g_camera3D.SetPosition(cameraPos);

	m_animeModel.Update(FRAMERATE_30);
	m_sprite2.Update(m_position, m_rot, m_scale, m_pivot);
	m_skinModel.UpdateWorldMatrix(CVector3::Zero(), m_rot, m_scale);
	m_skinModelStage.UpdateWorldMatrix(CVector3::Zero(), m_rot, m_scale);
}

void GameOver::Draw()
{
	m_skinModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
	m_skinModelStage.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void GameOver::PostDraw()
{
	m_sprite2.Draw();
}

