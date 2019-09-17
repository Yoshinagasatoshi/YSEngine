#include "stdafx.h"
#include "Player.h"
#include <iostream>
#include "gameObject/ysGameObjectManager.h"
Player::Player()
{
	//cmoファイルの読み込み。
	m_playerModel.Init(L"Assets/modelData/busyo.cmo");
}


Player::~Player()
{
}

void Player::Update()
{
	//左のスティック操作・横方向
	float WideMove = g_pad->GetLStickXF();
	WideMove *= 20.0f;
	m_position.x += WideMove;
	//左のスティック操作・縦方向
	float TateMove = g_pad->GetLStickYF();
	TateMove *= 20.0f;
	m_position.z += TateMove;
	if (g_pad->IsTrigger(enButtonA)) {
		m_position.y += 200.0f;
	}
	if (m_position.y > 0.0f) {
		m_position.y -= 30.0f;
	}
	//deleteがちゃんとできているかどうか試し
	//リクエストを送っているだけなのでunitychanは消えない
	timer++;
	if (timer > 100)
	{
		g_goMgr.DeleteGOObject(this);
	}
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