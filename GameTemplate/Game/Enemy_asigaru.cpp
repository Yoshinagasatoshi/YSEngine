#include "stdafx.h"
#include "Enemy_asigaru.h"
#include "gameObject/ysGameObjectManager.h"

Enemy_asigaru::IdlePos Enemy_asigaru::m_idlePos[5];
//コンストラクタ
Enemy_asigaru::Enemy_asigaru()
{
	//asigaruのモデルをロードする。
	m_model.Init(L"Assets/modelData/asigaru.cmo");
	m_rotation = CQuaternion::Identity();
	m_scale = CVector3::One();
	//アニメーションを格納だ
	m_asigaruAnimeClip[0].Load(L"Assets/animData/asigaru_totugeki.tka");
	m_asigaruAnimeClip[1].Load(L"Assets/animData/asigaru_tikazuki.tka");
	m_asigaruAnimeClip[2].Load(L"Assets/animData/asigaru_Normal_Attack.tka");

	m_asigaruAnimeClip[0].SetLoopFlag(true);
	m_asigaruAnimeClip[1].SetLoopFlag(true);
	m_asigaruAnimeClip[2].SetLoopFlag(true);

	m_asigaruAnime.Init(
		m_model,
		m_asigaruAnimeClip,
		3
	);
}
//デストラクタ
Enemy_asigaru::~Enemy_asigaru()
{
}

void Enemy_asigaru::CharaconInit()
{
	//重てぇ！！！！！！！！！！！！
	//キャラコンの初期化
	m_characon.Init(
		3.0f,
		3.0f,
		m_position
	);
}
void Enemy_asigaru::Update()
{
	if (!s) {
		s = true;
		CharaconInit();
	}
	idlePosInit();
	LenghtJudge();
	Move();
	
	//ワールド座標の更新
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_position = m_characon.Execute(1.0 / 30.0f,m_moveSpeed);

	if (g_pad->IsTrigger(enButtonA)) {
		m_asigaruAnime.Play(0, 0.2f);
	}
	//テストコード、ちゃんと消せるかどうか確認しているだけ
	//条件が統一されているので、全員消えてしまう。
	else if (g_pad->IsTrigger(enButtonUp)) {
		g_goMgr.DeleteGOObject(this);
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
	switch (m_state)
	{
	case Asigaru_idle:
		for (int i = 0; i < 5; i++) {
			if (!m_idlePos[i].m_isUse) {
				m_idlePos[i].m_isUse = true;
				moveV = m_idlePos[i].idlePos - m_position;
				break;
			}
		};
		moveV.y = 0.0f;
		moveV.Normalize();
		m_moveSpeed = moveV * 100.0f;
		break;
	case Asigaru_totugeki:
		m_moveSpeed = moveV * 100.0f;
		break;
	case Asigaru_sentou:
		m_moveSpeed = moveV * 100.0f;
		break;
	}
	m_moveSpeed.y += -100.0f;
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
	m_idlePos[4].idlePos = m_playerPos + CVector3{ 0.0f,0.0f,0.0f };
}
//距離による判定処理
void Enemy_asigaru::LenghtJudge()
{
	//for (int i = 0; i < 5; i++) {
	   	CVector3 a = m_idlePos[i].idlePos - m_position;
		float b = a.Length();
		if (b < 200.0f) {
			//m_position = m_idlePos[i].idlePos;
			m_state = Asigaru_sentou;
			m_asigaruAnime.Play(2, 0.2f);
			//break;
		}
		else if (b < 400.0f) {
			m_state = Asigaru_totugeki;
			m_asigaruAnime.Play(1, 0.2f);
			//break;
		//}
	}
}