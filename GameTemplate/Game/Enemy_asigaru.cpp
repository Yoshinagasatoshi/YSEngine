#include "stdafx.h"
#include "Enemy_asigaru.h"

//コンストラクタ
Enemy_asigaru::Enemy_asigaru()
{
	//asigaruのモデルをロードする。
	m_model.Init(L"Assets/modelData/asigaru.cmo");
	m_rotation = CQuaternion::Identity();
	m_scale = CVector3::One();
	m_asigaruAnimeClip[0].Load(L"Assets/animData/asigaru_totugeki.tka");
	m_asigaruAnimeClip[0].SetLoopFlag(true);
	m_asigaruAnime.Init(
		m_model,
		m_asigaruAnimeClip,
		1
	);
}
//デストラクタ
Enemy_asigaru::~Enemy_asigaru()
{

}
void Enemy_asigaru::Update()
{
	Move();
	//ワールド座標の更新
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	if (g_pad->IsTrigger(enButtonA)) {
		m_asigaruAnime.Play(0, 0.2f);
	}
	m_asigaruAnime.Update(1.0f / 30.0f);
}
void Enemy_asigaru::Draw()
{
	//モデルの描画
	m_model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}
void Enemy_asigaru::Move()
{
	m_playerPos = m_player->GetPosition();
	//モデルの移動　Gameクラスからm_player
	CVector3 moveV;
	moveV = m_playerPos - m_position;
	moveV.y = 0.0f;
	moveV.Normalize();
	CVector3 moveSpeed = moveV * 10.0f;
	m_position += moveSpeed;
}