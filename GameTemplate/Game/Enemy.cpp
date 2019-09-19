#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "gameObject/ysGameObjectManager.h"
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
	Move();
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

void Enemy::Move()
{
	//モデルの移動これも仮。...仮多い
	//GameDataクラスを通してプレイヤーのインスタンスを探した方がいいのか
	//FIndGo使って探したほうがいいのかわからんとりあえず仮
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