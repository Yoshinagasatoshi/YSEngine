#include "stdafx.h"
#include "Player.h"
#include <iostream>
#include "gameObject/ysGameObjectManager.h"


Player::Player()
{
	CharaconInit();
	//cmo�t�@�C���̓ǂݍ��݁B
	m_playerModel.Init(L"Assets/modelData/busyo.cmo");
}


Player::~Player()
{
}

void Player::CharaconInit()
{
	m_characon.Init(
		20.0f,
		20.0f,
		m_position
	);
}
void Player::Update()
{
	//���̃X�e�B�b�N����E������
	float WideMove = g_pad->GetLStickXF();
	WideMove *= -20.0f;
	m_moveSpeed.x += WideMove;
	//���̃X�e�B�b�N����E�c����
	float TateMove = g_pad->GetLStickYF();
	TateMove *= -20.0f;
	m_moveSpeed.z += TateMove;
	if (g_pad->IsTrigger(enButtonA)) {
		m_moveSpeed.y += 200.0f;
	}
	if (m_position.y > 0.0f) {
		m_moveSpeed.y -= 30.0f;
	}
	//GameTime���˂�MainCamera���˂�
	//��ő��₳�Ȃ��Ⴞ���ǉ��ŏ����Ă邱�̃R�[�h
	m_position = m_moveSpeed;
	//���[���h�s��̍X�V�B
	m_playerModel.UpdateWorldMatrix(m_position,m_rotation,m_scale);
}
void Player::Draw()
{
	m_playerModel.Draw(
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
}