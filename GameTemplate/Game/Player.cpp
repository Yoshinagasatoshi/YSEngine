#include "stdafx.h"
#include "Player.h"


Player::Player()
{
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model.Init(L"Assets/modelData/unityChan.cmo");
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
	//���[���h�s��̍X�V�B
	m_model.UpdateWorldMatrix(m_position,m_rotation,m_scale);
}
void Player::Draw()
{
	m_model.Draw(
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
}