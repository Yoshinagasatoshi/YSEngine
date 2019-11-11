#include "stdafx.h"
#include "GameData.h"
/// <summary>
/// ���Ӄ��X�g�N���X
/// �����ɍ��W�n���ӂ��J���Ă��邩
/// �₢���킹��N���X
/// �������J���Ă��Ȃ����
/// ���y�͎��ӂ����낤�낵�Ă���B
/// </summary>
GameData::GameData()
{
	/// <summary>
	/// �J�����̕������l�������ʒu�ɐݒ肵�����̂�
	/// g_camera3D����������������Ă���
	/// </summary>
	const float kyori = 100.0f;
	const float half = 50.0f;

	CVector3 CameraForword = g_camera3D.GetForword();
	CVector3 CameraRight = g_camera3D.GetRight();

	CameraForword.y = 0.0f;
	CameraRight.y = 0.0f;

	CameraForword.Normalize();
	CameraRight.Normalize();
}

GameData::~GameData()
{

}

void GameData::Update()
{
	kakomiUpdate();
}

void GameData::kakomiUpdate()
{
	EnemySpot[0] = m_player->GetPosition() + CameraForword * kyori;
	EnemySpot[1] = m_player->GetPosition() + CameraRight * kyori;
	EnemySpot[2] = m_player->GetPosition() + CameraForword * half;
	EnemySpot[3] = m_player->GetPosition() + CameraRight * half;
	EnemySpot[4] = m_player->GetPosition();
}

void GameData::Draw()
{

}