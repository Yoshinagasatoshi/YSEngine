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
	//�L�����R���̏�����
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
	WideMove *= -200.0f;
	m_moveSpeed.x = WideMove;
	//���̃X�e�B�b�N����E�c����
	float TateMove = g_pad->GetLStickYF();
	TateMove *= -200.0f;
	m_moveSpeed.z = TateMove;
	if (g_pad->IsTrigger(enButtonA)) {
		m_moveSpeed.y = 200.0f;
	}
	m_moveSpeed.y -= 15.0f;
	//���[���h�s��̍X�V�B
	m_position = m_characon.Execute(1.0/30.0f,m_moveSpeed);
	m_playerModel.UpdateWorldMatrix(m_position,m_rotation,m_scale);
}
void Player::Draw()
{
	//�v���C���[��`��
	m_playerModel.Draw(
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
}