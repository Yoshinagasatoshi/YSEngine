#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "Fade.h"
#include "gameObject/ysGameObjectManager.h"
#include "InGameSoundDirector.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

const float BGM_VOL = 0.5f; //BGM�̃{�����[���B���l�ɂ���ĕύX�\
const float Camera_Z_pos = -600.0f;	//�J�������ǂꂾ���Ђ����Ă��邩

Title::Title()
{
	m_soundEngine.Init();
	//��SE�f��
	//m_bgm.Init(L"Assets/sound/Hokora.wav");
	//m_bgm.Play(true);
	//m_bgm.SetVolume(BGM_VOL);

	//m_sprite = new Sprite();
	m_sprite.Init(L"Assets/sprite/Title.dds", FRAME_BUFFER_W, FRAME_BUFFER_H);

	CVector3 CameraPos = { 0.0f,0.0f,Camera_Z_pos };
	g_camera2D.SetWidth(FRAME_BUFFER_W);
	g_camera2D.SetHeight(FRAME_BUFFER_H);
	g_camera2D.SetPosition({ CameraPos });
	g_camera2D.SetTarget(CVector3::Zero());
	g_camera2D.Update();
	Fade::Getinstance().StartFadeOut();
}

Title::~Title()
{
	m_bgm.Release();
	//g_goMgr.DeleteGOObject(this);
	//m_sprite.~Sprite();
}

void Title::Update()
{
	//���X�ɉ��𗎂Ƃ��悤��
	if (g_pad->IsTrigger(enButtonA)) 
	{
		m_isWaitFadeOut = true;
	}
	if (m_isWaitFadeOut) {
		Fade::Getinstance().StartFadeIn();
		if (!Fade::Getinstance().IsFade()) {
			//m_bgm.Stop();
			InGameSoundDirector::GetInstans().UpdateOn();
			/*Game* game = */g_goMgr.NewGameObject<Game>("Game");
			g_goMgr.DeleteGOObject(this);
		}
	}

}

void Title::Draw()
{
	m_sprite.Draw();
}