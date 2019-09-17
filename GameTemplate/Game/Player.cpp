#include "stdafx.h"
#include "Player.h"
#include <iostream>
#include "gameObject/ysGameObjectManager.h"
Player::Player()
{
	//cmo�t�@�C���̓ǂݍ��݁B
	m_playerModel.Init(L"Assets/modelData/busyo.cmo");
}


Player::~Player()
{
}

void Player::Update()
{
	//���̃X�e�B�b�N����E������
	float WideMove = g_pad->GetLStickXF();
	WideMove *= 20.0f;
	m_position.x += WideMove;
	//���̃X�e�B�b�N����E�c����
	float TateMove = g_pad->GetLStickYF();
	TateMove *= 20.0f;
	m_position.z += TateMove;
	if (g_pad->IsTrigger(enButtonA)) {
		m_position.y += 200.0f;
	}
	if (m_position.y > 0.0f) {
		m_position.y -= 30.0f;
	}
	//delete�������Ƃł��Ă��邩�ǂ�������
	//���N�G�X�g�𑗂��Ă��邾���Ȃ̂�unitychan�͏����Ȃ�
	timer++;
	if (timer > 100)
	{
		g_goMgr.DeleteGOObject(this);
	}
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