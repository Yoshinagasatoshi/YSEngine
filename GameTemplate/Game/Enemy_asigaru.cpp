#include "stdafx.h"
#include "Enemy_asigaru.h"

//�R���X�g���N�^
Enemy_asigaru::Enemy_asigaru()
{
	//asigaru�̃��f�������[�h����B
	m_model.Init(L"Assets/modelData/asigaru.cmo");
	m_rotation = CQuaternion::Identity();
	m_scale = CVector3::One();
	m_asigaruAnimeClip[0].Load(L"Assets/animData/asigaru_totugeki.tka");
	m_asigaruAnimeClip[0].SetLoopFlag(true);
	m_asigaruAnime.Init(
		m_model,
		m_asigaruAnimeClip,
		1
	);
}
//�f�X�g���N�^
Enemy_asigaru::~Enemy_asigaru()
{

}
void Enemy_asigaru::Update()
{
	Move();
	//���[���h���W�̍X�V
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	if (g_pad->IsTrigger(enButtonA)) {
		m_asigaruAnime.Play(0, 0.2f);
	}
	m_asigaruAnime.Update(1.0f / 30.0f);
}
void Enemy_asigaru::Draw()
{
	//���f���̕`��
	m_model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}
void Enemy_asigaru::Move()
{
	m_playerPos = m_player->GetPosition();
	//���f���̈ړ��@Game�N���X����m_player
	CVector3 moveV;
	moveV = m_playerPos - m_position;
	moveV.y = 0.0f;
	moveV.Normalize();
	CVector3 moveSpeed = moveV * 10.0f;
	m_position += moveSpeed;
}