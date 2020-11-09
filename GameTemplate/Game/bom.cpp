#include "stdafx.h"
#include "bom.h"
#include "gameObject/ysGameObjectManager.h"
#include "Fade.h"
#include "InGameSoundDirector.h"
const float BOM_RADIUS = 30.0f;			//�S�[�X�g�̓����蔻��̑傫���p
const float BOM_GRAVITY_ACC = -1000.0f;	//���e�̏d�͉����x�B
const float BOM_VELOCITY_HORIZON = 200;	//���e�̐��������̈ړ����x�B
const float BOM_SCALE = 3.0f;			//���e�̑傫�����������B

const float BOM_Displacement = 0.5f;	//�{���̕ψʂɎg�����l

const float BOM_HIT_AREA = 150.0f;		//�{�����v���C���[������͈�
const float BOM_LANDING_AREA = 100.0f;	//�{�����n�ʂɒ��n�����ƔF������͈�

Bom::Bom()
{
	//�{�������ݒ�
	m_skinModel.Init(L"Assets/modelData/bom.cmo");
	m_skinModel.SetShadowCaster(false);
	m_ghostObject.CreateSphere(
		m_position,
		m_rotation,
		BOM_RADIUS
	);
}

Bom::~Bom()
{
	g_goMgr.DeleteGOObject(this);
}

void Bom::Update()
{
	//�t�F�[�h���(�V�[���̐؂�ւ������Ă�����)����Ȃ����
	//���e�Ƃ��Ă̏���������
	if (Fade::Getinstance().IsFade()) {
		if (!m_isFirst) {
			FirstSet();//�ŏ��̃{���̏�Ԃ����

			//CSoundSource* se = new CSoundSource;
			//se->Init(L"Assets/sound/fuse.wav");
			//se->Play(false);
			//se->SetVolume(0.1f);

			//InGameSoundDirector::GetInstans().RingSE_Fuse();
		}

		//if (g_goMgr) {
		HitThebom();
		Finalbom();
		//�d�͉����x�����Z
		m_bomVelocity.y += BOM_GRAVITY_ACC * GameTime().GetFrameDeltaTime();

		m_position += m_bomVelocity * GameTime().GetFrameDeltaTime();

		m_skinModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		//�{���̔���ɂԂꂪ���邽�ߏ�����ɏC��
		//m_bomGhostPos = m_position;
		//m_bomGhostPos.y += B_radius * 0.5f;
		//m_ghostObject.SetPosition(m_position);
	//}
	//else {
	//	DeleteGO(this);
	//}
	}
	//�V�[���̐؂�ւ������Ă��鎞�ɂ͂���
	//�v���C���[���̃C���X�^���X���Q�Ƃ����肷�鏈�������Ăق����Ȃ��̂�
	//�{�����ƃf���[�g����
	else {
		DeleteGO(this);
	}
}

void Bom::Draw()
{
	//���f���̕`��
	m_skinModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

//��ԍŏ��Ƀ{���̐ݒ������B
//�ŏ��̓v���C���[�ɒ��ړ�����̂ł͂Ȃ��A���̒��p�_��ڎw���O��������B
void Bom::FirstSet()
{
	CVector3 vec = m_inpactPoint - m_position;
	vec.y = 0.0f;//Y�̐���������
	vec.Normalize();
	vec *= BOM_VELOCITY_HORIZON;
	
	CVector3 distans = m_inpactPoint - m_position;
	distans.y = 0.0f;	//Y�̐���������
	float a = BOM_GRAVITY_ACC * BOM_Displacement;
	float x =  m_inpactPoint.y - m_position.y;
	float t = distans.Length() / vec.Length();
	float Vo = (x - a * t * t) / t;
	vec.y = Vo;

	m_bomVelocity = vec;

	m_rotation = CQuaternion::Identity();//���̂Ȃ̂Ō����Ă�������͋C�ɂ��Ȃ�
	m_scale = CVector3::One() * BOM_SCALE; //���e���f���������������̂ŁA3�{�̑傫���ɂ����B
	m_isFirst = true;
}

void Bom::Finalbom()
{
	auto I_diff = m_inpactPoint - m_position;
	//���n�����B�v���C���[�ɓ������Ă��Ȃ����̏���
	if (I_diff.Length() < BOM_LANDING_AREA) {
		//CSoundSource* se = new CSoundSource;
		//se->Init(L"Assets/sound/destruction.wav");
		//se->Play(false);
		//se->SetVolume(0.1f);
		g_Effect.m_sampleEffect = Effekseer::Effect::Create(g_Effect.m_effekseerManager, (const EFK_CHAR*)L"Assets/effect/bom.efk");
		//�G�t�F�N�g���Đ�����B
		g_Effect.m_playEffectHandle = g_Effect.m_effekseerManager->Play(g_Effect.m_sampleEffect, m_position.x,m_position.y,m_position.z);

		DeleteGO(this);
	}
	Effekseer::Matrix44 efCameraMat;
	g_camera3D.GetViewMatrix().CopyTo(efCameraMat);
	Effekseer::Matrix44 efProjMat;
	g_camera3D.GetProjectionMatrix().CopyTo(efProjMat);
	g_Effect.m_effekseerRenderer->SetCameraMatrix(efCameraMat);
	g_Effect.m_effekseerRenderer->SetProjectionMatrix(efProjMat);
	g_Effect.m_effekseerManager->Update();
}

void Bom::HitThebom()
{
	//�v���C���[�Ƀ{���������������̏���
	if (!m_player->GetPlayerDead()) {
		if (m_player == nullptr) {
			DeleteGO(this);
		}
		auto P_diff = m_player->GetPosition() - m_position;
		if (P_diff.Length() < BOM_HIT_AREA)
		{
			m_player->PlayerDamage();

			//InGameSoundDirector::GetInstans().RingSE_Destruct();

			//CSoundSource* se = new CSoundSource;
			//se->Init(L"Assets/sound/destruction.wav");
			//se->Play(false);
			//se->SetVolume(0.1f);

			g_Effect.m_sampleEffect = Effekseer::Effect::Create(g_Effect.m_effekseerManager, (const EFK_CHAR*)L"Assets/effect/bom.efk");
			//�G�t�F�N�g���Đ�����B
			g_Effect.m_playEffectHandle = g_Effect.m_effekseerManager->Play(g_Effect.m_sampleEffect, m_position.x, m_position.y, m_position.z);

			DeleteGO(this);
		}
		Effekseer::Matrix44 efCameraMat;
		g_camera3D.GetViewMatrix().CopyTo(efCameraMat);
		Effekseer::Matrix44 efProjMat;
		g_camera3D.GetProjectionMatrix().CopyTo(efProjMat);
		g_Effect.m_effekseerRenderer->SetCameraMatrix(efCameraMat);
		g_Effect.m_effekseerRenderer->SetProjectionMatrix(efProjMat);
		g_Effect.m_effekseerManager->Update();
	}
	else {
		DeleteGO(this);
	}
}