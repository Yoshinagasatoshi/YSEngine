#include "stdafx.h"
#include "Player.h"
#include <iostream>
#include "gameObject/ysGameObjectManager.h"


Player::Player()
{
	CharaconInit();
	//cmoファイルの読み込み。
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
	//左のスティック操作・横方向
	float WideMove = g_pad->GetLStickXF();
	WideMove *= -20.0f;
	m_moveSpeed.x += WideMove;
	//左のスティック操作・縦方向
	float TateMove = g_pad->GetLStickYF();
	TateMove *= -20.0f;
	m_moveSpeed.z += TateMove;
	if (g_pad->IsTrigger(enButtonA)) {
		m_moveSpeed.y += 200.0f;
	}
	if (m_position.y > 0.0f) {
		m_moveSpeed.y -= 30.0f;
	}
	//GameTimeもねえMainCameraもねえ
	//後で増やさなきゃだけど仮で書いてるこのコード
	m_position = m_moveSpeed;
	//ワールド行列の更新。
	m_playerModel.UpdateWorldMatrix(m_position,m_rotation,m_scale);
}
void Player::Draw()
{
	m_playerModel.Draw(
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
}