#include "stdafx.h"
#include "Enemy_asigaru.h"
#include "gameObject/ysGameObjectManager.h"
#include "GameData.h"

Enemy_asigaru::IdlePos Enemy_asigaru::m_idlePos[5];
//コンストラクタ
Enemy_asigaru::Enemy_asigaru()
{
	//asigaruのモデルをロードする。
	m_model.Init(L"Assets/modelData/asigaru.cmo");
	m_rotation = CQuaternion::Identity();
	m_scale = CVector3::One();
	//アニメーションを格納だ
	m_asigaruAnimeClip[Asigaru_totugeki].Load(L"Assets/animData/asigaru_totugeki.tka");
	m_asigaruAnimeClip[Asigaru_tikazuki].Load(L"Assets/animData/asigaru_tikazuki.tka");
	m_asigaruAnimeClip[Asigaru_sentou].Load(L"Assets/animData/asigaru_Normal_Attack.tka");
	m_asigaruAnimeClip[Asigaru_damage].Load(L"Assets/animData/asigaru_damage.tka");
	m_asigaruAnimeClip[Asigaru_dead].Load(L"Assets/animData/asigaru_dead.tka");

	m_asigaruAnimeClip[Asigaru_totugeki].SetLoopFlag(true);
	m_asigaruAnimeClip[Asigaru_tikazuki].SetLoopFlag(true);
	m_asigaruAnimeClip[Asigaru_sentou].SetLoopFlag(false);
	m_asigaruAnimeClip[Asigaru_damage].SetLoopFlag(false);
	m_asigaruAnimeClip[Asigaru_dead].SetLoopFlag(false);

	m_asigaruAnime.Init(
		m_model,
		m_asigaruAnimeClip,
		Asigaru_anim_num
	);

	m_asigaruAnime.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName) {
		(void)clipName;
		MessageBox(NULL, "attack", "attack", MB_OK);
		}
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
	const float grabity = -30.0f;
	if (!m_characonState) {
		m_characonState = true;
		CharaconInit();
	}
	//Move();
	Turn();
	StateJudge();
	
	m_moveSpeed.y = grabity;
	//ワールド座標の更新
	m_position = m_characon.Execute(1.0f / 30.0f, m_moveSpeed);
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
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
	const int NomalMove = -1;
	int kakoi_num = m_player->RequestEnemyData(m_position, this);
	//周辺を取り巻くやつらなのか？
	if (kakoi_num != NomalMove) {
		//ここから下は取り巻くやつの処理
		CVector3 kaiten = m_playerPos - m_position;
		//ここだけ動きと回転の処理を変える
		idlePosInit();
		m_moveSpeed = m_idlePos[kakoi_num].idlePos - m_position;
		m_moveSpeed.Normalize();
		m_moveSpeed *= 30.0f;
		float angle = atan2(kaiten.x,kaiten.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
		return;
	}
	else {
		/// <summary>
		/// ランダムなポイントを指して取り巻きのちょっと後ろ位を
		/// うろうろしている感じ
		/// </summary>
	}
}

void Enemy_asigaru::Turn()
{
	auto Rot = CMatrix::Identity();
	Rot.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = Rot.m[2][0];
	m_forward.y = Rot.m[2][1];
	m_forward.z = Rot.m[2][2];

	if (fabsf(m_moveSpeed.x) <= 0.001f    //fabsfは絶対値。m_movespeed.x&m_movespeedzが
		&& fabsf(m_moveSpeed.z) <= 0.001f) {//0.001以下の時には何もしない。
		return;
	}
	else {
		float angle = atan2(m_moveSpeed.x, m_moveSpeed.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
	}
}
//周りを囲むプレイヤーの周りを囲む場所の情報
void Enemy_asigaru::idlePosInit()
{
	//４点を作ってそこにエネミーを群がらせる
	//レベルでやった方がいいというアドヴァイスをもらったので
	//後でレベルに変える。
	m_playerPos = m_player->GetPosition();

	m_idlePos[0].idlePos = m_playerPos + CVector3{100.0f,0.0f,0.0f};
	m_idlePos[1].idlePos = m_playerPos + CVector3{ -100.0f,0.0f,0.0f };
	m_idlePos[2].idlePos = m_playerPos + CVector3{ 100.0f,0.0f,100.0f };
	m_idlePos[3].idlePos = m_playerPos + CVector3{ -100.0f,0.0f,100.0f };
	m_idlePos[4].idlePos = m_playerPos + CVector3{ 0.0f,0.0f,-100.0f };
}

//距離による判定処理
void Enemy_asigaru::StateJudge()
{
	//～～Range
	const float BattleRange = 150.0f * 150.0f;
	const float VililanceRange = 500.0f * 500.0f;
	m_playerPos = m_player->GetPosition();
	CVector3 kyori = m_playerPos - m_position;
	//y成分を0にする。
	kyori.y = 0.0f;

	//ステートごとの処理を書く
	switch (m_asigaruState)
	{
	case Asigaru_sentou:
	if (kyori.LengthSq() > BattleRange) {
		m_asigaruState = Asigaru_tikazuki;
	}
	//sentouの処理
	m_moveSpeed = CVector3::Zero();
		Move();
		m_player->PlayerDamage();
		break;


	case Asigaru_tikazuki:
		if (kyori.LengthSq() < BattleRange) {
			kyori.Normalize();
			float angle = acosf(kyori.Dot(m_forward));
			//視野角入ってる？
			if (fabsf(angle) < PI * 0.25f) {
					m_asigaruState = Asigaru_sentou;
			}
		}
		else if (kyori.LengthSq() > VililanceRange)
		{
			m_asigaruState = Asigaru_totugeki;
		}
		//tikazukiの処理
		kyori.Normalize();
		m_moveSpeed = kyori * 50.0f;
		break;


	case Asigaru_totugeki:
		if (kyori.LengthSq() < VililanceRange) {
			//視野角入ってる？
			kyori.Normalize();
			float angle = acosf(kyori.Dot(m_forward));
			if (fabsf(angle) < PI * 0.25f) {
				m_asigaruState = Asigaru_tikazuki;
			}
		}
		//totugekiの処理
		m_moveSpeed = CVector3::Zero();
		CQuaternion ADDrot = CQuaternion::Identity();
		ADDrot.SetRotationDeg(CVector3::AxisY(), 2.0f);
		m_rotation.Multiply(ADDrot);
		break;
	}
	m_asigaruAnime.Play(m_asigaruState, 0.2f);
}