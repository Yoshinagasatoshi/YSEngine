#include "stdafx.h"
#include "Enemy.h"
Enemy::Enemy()
{
	//asigaru�̃��f�������[�h����B
	m_model.Init(L"Assets/modelData/asigaru.cmo");
}

Enemy::~Enemy()
{
	
}

void Enemy::Update()
{
	//���[���h���W�̍X�V
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}

void Enemy::Draw()
{
	//���f���̕`��
	m_model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}