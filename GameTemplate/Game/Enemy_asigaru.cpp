#include "stdafx.h"
#include "Enemy_asigaru.h"

Enemy_asigaru::IdlePos Enemy_asigaru::m_idlePos[5];
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
	idlePosInit();
	Move();
	LenghtJudge();
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
int time = 0;
void Enemy_asigaru::Move()
{
	//m_playerPos = m_player->GetPosition();
	////モデルの移動　Gameクラスからm_player
	//CVector3 moveV;
	//moveV = m_playerPos - m_position;
	//moveV.y = 0.0f;
	//moveV.Normalize();
	//CVector3 moveSpeed = moveV * 10.0f;
	//m_position += moveSpeed;
	m_playerPos = m_player->GetPosition();
	for (int i = 0; i < 5; i++) {
		if (!m_idlePos[i].m_isUse) {
			m_idlePos[i].m_isUse = true;
			moveV = m_idlePos[i].idlePos - m_position;
			break;
		}
	};
	idlePosInit();
	moveV.y = 0.0f;
	moveV.Normalize();
	moveSpeed = moveV * 10.0f;
	m_position += moveSpeed;
}
//周りを囲むプレイヤーの周りを囲む場所の情報
void Enemy_asigaru::idlePosInit()
{
	//４点を作ってそこにエネミーを群がらせる
	//レベルでやった方がいいというアドヴァイスをもらったので
	//後でレベルに変える。
	m_playerPos = m_player->GetPosition();
	m_idlePos[0].idlePos = m_playerPos + CVector3{ 0.0f,0.0f,100.0f };
	m_idlePos[1].idlePos = m_playerPos + CVector3{ 0.0f,0.0f,-100.0f };
	m_idlePos[2].idlePos = m_playerPos + CVector3{ -100.0f,0.0f,0.0f };
	m_idlePos[3].idlePos = m_playerPos + CVector3{ 100.0f,0.0f,0.0f };
	m_idlePos[4].idlePos = m_playerPos + CVector3{ 0.0f,100.0f,0.0f };
}
//距離による判定処理
void Enemy_asigaru::LenghtJudge()
{
	for (int i = 0; i < 5; i++) {
	   	CVector3 a = m_idlePos[i].idlePos - m_position;
		float b = a.Length();
		if (b < 50.0f) {
			m_position = m_idlePos[i].idlePos;
			break;
		}
	}
}