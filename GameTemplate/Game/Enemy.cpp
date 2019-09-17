#include "stdafx.h"
#include "Enemy.h"
Enemy::Enemy()
{
	//asigaruのモデルをロードする。
	m_model.Init(L"Assets/modelData/asigaru.cmo");
}

Enemy::~Enemy()
{
	
}

void Enemy::Update()
{
	//ワールド座標の更新
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}

void Enemy::Draw()
{
	//モデルの描画
	m_model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}