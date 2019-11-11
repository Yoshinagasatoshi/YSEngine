#include "stdafx.h"
#include "GameData.h"
/// <summary>
/// 周辺リストクラス
/// こいつに座標系周辺が開いているか
/// 問い合わせるクラス
/// こいつが開いていなければ
/// 足軽は周辺をうろうろしている。
/// </summary>
GameData::GameData()
{
	/// <summary>
	/// カメラの方向を考慮した位置に設定したいので
	/// g_camera3Dから情報を引っ張っている
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