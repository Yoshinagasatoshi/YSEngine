#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "gameObject/ysGameObjectManager.h"
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
	Move();
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

void Enemy::Move()
{
	//���f���̈ړ���������B...������
	//GameData�N���X��ʂ��ăv���C���[�̃C���X�^���X��T�������������̂�
	//FIndGo�g���ĒT�����ق��������̂��킩���Ƃ肠������
	CVector3 moveV;
	if (timer < 100) {
		moveV = { 5.0f,0.0f,0.0f };
	}
	else if(timer < 200) {
	    moveV = { -5.0f,0.0f,0.0f };
	}
	else {
		timer = 0;
	}
	moveV.y = 0.0f;
	moveV.Normalize();
	CVector3 moveSpeed = moveV * 10.0f;
	m_position += moveSpeed;
	timer++;
}