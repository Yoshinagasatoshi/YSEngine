#include "stdafx.h"
#include "GameClear.h"
#include "gameObject/ysGameObjectManager.h"
#include "Title.h"
#include "Fade.h"
//#include "sound/SoundEngine.h"
//#include "sound/SoundSource.h"
#include "InGameSoundDirector.h"
const float SpriteH = 240.0f; //�摜�c
const float halfVol = 0.5f;		//�����̉��ʁA���ʂ��傫��wave�t�@�C���͂����Œ���
const float INTERPORATION_TIME_S = 0.1f;//��Ԏ��ԁE���A�Z�߂̃A�j���[�V������Ԃ��������Ƃ��Ɏg���B 
const float FRAMERATE_30 = 1.0f / 30.0f;//�t���[�����[�g30�@30fps

const CVector3 cameraPos = { 0.0f,200.0f,300.0f };	//�J�����ʒu�̒���
const CVector3 targetPos = { 0.0f,100.0f,0.0f };	//�J���������_�̒���
GameClear::GameClear()
{
	m_soundEngine.Init();
	//��SE�f��
	m_bgm.Init(L"Assets/sound/StageClear.wav");
	m_bgm.Play(true);
	m_bgm.SetVolume(halfVol);

	m_skinModel.Init(L"Assets/modelData/busyo.cmo");
	m_skinModelStage.Init(L"Assets/modelData/result_stage.cmo");

	m_animClip[anim_Win].Load(L"Assets/animData/busyo_katidoki.tka");
	m_animClip[anim_Win].SetLoopFlag(false);

	m_sprite1.Init(L"Assets/sprite/Win.dds",FRAME_BUFFER_W, SpriteH);
	//m_sprite2.Init(L"Assets/sprite/Win2.dds", 960.0f, 360.0f);
	Fade::Getinstance().StartFadeOut();

	//�A�j���[�V�����惂�f���ƃA�j���[�V�����̌���ݒ�B
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
	m_animeModel.Update(FRAMERATE_30);
	m_skinModel.UpdateWorldMatrix(CVector3::Zero(), m_rot, m_scale);
	m_skinModelStage.UpdateWorldMatrix(CVector3::Zero(), m_rot, m_scale);
}

void GameClear::Draw()
{
	//�X�e�[�W�N���A�p�̃��f��2�������f���Ă���
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