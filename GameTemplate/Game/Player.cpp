#include "stdafx.h"
#include "Player.h"
#include <iostream>
#include "gameObject/ysGameObjectManager.h"


Player::Player()
{
	CharaconInit();
	//cmo�t�@�C���̓ǂݍ��݁B
	m_playerModel.Init(L"Assets/modelData/busyo.cmo");
	m_busyoAnimeClip[animClip_idle].Load(L"Assets/animData/busyo_idle.tka");
	m_busyoAnimeClip[animClip_idle].SetLoopFlag(true);
	//�U���A�j�����[�h
	m_busyoAnimeClip[animClip_ATK1].Load(L"Assets/animData/busyo_kougeki.tka");
	m_busyoAnimeClip[animClip_ATK2].Load(L"Assets/animData/busyo_kougeki2.tka");
	m_busyoAnimeClip[animClip_ATK3].Load(L"Assets/animData/busyo_kougeki3.tka");
	m_busyoAnimeClip[animClip_ATK4].Load(L"Assets/animData/busyo_kougeki4.tka");
	m_busyoAnimeClip[animClip_ATK5].Load(L"Assets/animData/busyo_kougeki5.tka");
	//�S�����̍Đ��ł����̂�false�ɂ��Ƃ�
	for (int i = animClip_ATK1; i < animClip_num; i++) {
		m_busyoAnimeClip[i].SetLoopFlag(false);
	}
	//�A�j���[�V�����惂�f���ƃA�j���[�V�����̌���ݒ�B
	m_busyoAnime.Init(
		m_playerModel,
		m_busyoAnimeClip,
		animClip_num
	);
}


Player::~Player()
{
}

void Player::CharaconInit()
{
	//�L�����R���̏�����
	m_characon.Init(
		20.0f,
		20.0f,
		m_position
	);
}
void Player::Update()
{
	//���͗ʂ��󂯎��
	float WideMove   = g_pad->GetLStickXF();
	float heightMove = g_pad->GetLStickYF();
	
	//�J�����̑O�����ƉE�������擾
	CVector3 CameraForward = g_camera3D.GetForword();
	CVector3 CameraRight = g_camera3D.GetRight();
	//Y�̏��͂���Ȃ��̂�0�ɂ��A�O�ƉE�����̒P�ʂƂ���B
	CameraForward.y = 0.0f;
	CameraForward.Normalize();
	CameraRight.y = 0.0f;
	CameraRight.Normalize();
	m_moveSpeed = CVector3::Zero();
	m_moveSpeed += CameraForward * heightMove * 600.0f;
	m_moveSpeed += CameraRight * WideMove * 600.0f;
	m_moveSpeed.y -= 480.0f;
	//�n�ʂ��Ă�H
	if (m_characon.IsOnGround()) {
		m_moveSpeed.y = 0.0f;
		AttackMove();
		//�W�����v���Ă��H
		if (m_Jumpfrag) {
			m_Jumpfrag = false;
		}
	}
	//�X�e�[�g���Ƃɂ̏����Ɍ�ł���B
	if (g_pad->IsTrigger(enButtonA)) {
		if (!m_Jumpfrag) {
			m_moveSpeed.y = 6000.0f;
			m_Jumpfrag = true;
			i = 0;
		}
		m_busyoAnime.Play(animClip_idle, 0.5f);
	}
	Turn();
	//���[���h�s��̍X�V�B
	m_playerModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_position = m_characon.Execute(1.0f/60.0f,m_moveSpeed);
	m_busyoAnime.Update(1.0f / 30.0f);
}
void Player::Draw()
{
	//�v���C���[��`��
	m_playerModel.Draw(
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
}
void Player::Turn()
{
	if (fabsf(m_moveSpeed.x) <= 0.001f    //fabsf�͐�Βl�Bm_movespeed.x&m_movespeedz��
		&& fabsf(m_moveSpeed.z) <= 0.001f) {//0.001�ȉ��̎��ɂ͉������Ȃ��B
		return;
	}
	else {
		float angle = atan2(m_moveSpeed.x, m_moveSpeed.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
	}
}
void Player::AttackMove()
{
	if (g_pad->IsTrigger(enButtonB)&&timer>10.0f) {
		//���肵�܂��B
		//�X�g���e�W�[�p�^�[���\���R
		switch (i)
		{
		case 0:
			m_busyoAnime.Play(animClip_ATK1, 0.5f);
			i++;
			break;
		case 1:
			m_busyoAnime.Play(animClip_ATK2, 0.5f);
			i++;
			break;
		case 2:
			m_busyoAnime.Play(animClip_ATK3, 0.5f);
			i++;
			break;
		case 3:
			m_busyoAnime.Play(animClip_ATK4, 0.5f);
			i++;
			break;
		case 4:
			m_busyoAnime.Play(animClip_ATK5, 0.5f);
			i++;
			break;
		}
	}
	timer++;
	if (i != 0) {
		if (i != a) {
			timer = 0;
			a = i;
		}
		if (timer >= 40) {
			i = 0;
			a = 0;
			timer = 0;
			m_busyoAnime.Play(animClip_idle, 1.0f);
		}
	}
}