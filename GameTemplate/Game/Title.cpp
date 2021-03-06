#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "Fade.h"
#include "gameObject/ysGameObjectManager.h"
#include "InGameSoundDirector.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

const float BGM_VOL = 0.4f; //BGMのボリューム。数値によって変更可能
const CVector3 CAMERA_POS = CVector3 { 0.0f,0.0f,-600.0f };	//カメラをどれだけひかせているか

Title::Title()
{
	m_soundEngine.Init();
	//音SE素材
	m_bgm.Init(L"Assets/sound/Hokora.wav");
	m_bgm.Play(true);
	m_bgm.SetVolume(BGM_VOL);

	//m_sprite = new Sprite();
	m_sprite.Init(L"Assets/sprite/Title.dds", FRAME_BUFFER_W, FRAME_BUFFER_H);

	g_camera2D.SetWidth(FRAME_BUFFER_W);
	g_camera2D.SetHeight(FRAME_BUFFER_H);
	g_camera2D.SetPosition({ CAMERA_POS });
	g_camera2D.SetTarget(CVector3::Zero());
	g_camera2D.Update();
	Fade::Getinstance().StartFadeOut();
}

Title::~Title()
{
	m_bgm.Release();
	g_goMgr.DeleteGOObject(this);
	//m_sprite.~Sprite();
}

void Title::Update()
{
	//徐々に音を落とすように
	if (g_pad->IsTrigger(enButtonA)) 
	{
		m_isWaitFadeOut = true;
	}
	if (m_isWaitFadeOut) {
		Fade::Getinstance().StartFadeIn();
		if (!Fade::Getinstance().IsFade()) {
			//m_bgm.Stop();
			InGameSoundDirector::GetInstans().UpdateOn();
			g_goMgr.NewGameObject<Game>("Game");
			g_goMgr.DeleteGOObject(this);
		}
	}

}

void Title::Draw()
{
	m_sprite.Draw();
}