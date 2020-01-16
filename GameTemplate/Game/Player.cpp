#include "stdafx.h"
#include "Player.h"
#include <iostream>
#include "gameObject/ysGameObjectManager.h"
#include "Enemy.h"
#include "Enemy_asigaru.h"
#include "Wepon_ghost.h"

const float posClearRange = 600.0f * 600.0f;	//クリア判定を行う範囲。
const float PLAYER_COLLIDER_HEIGHT = 100.0f;		//プレイヤーのカプセルコライダーの高さ。
const float PLAYER_COLLIDER_RADIUS = 28.0f;		//プレイヤーのカプセルコライダーの半径。

const float SpeedAmount = 1000.0f;						//平面の移動量
const float gravity = 600.0f;								//重力
const float JumpPower = 1200.0f;							//プレイヤーの飛ぶ力

Player::Player()
{
	CharaconInit();
	//cmoファイルの読み込み。
	m_playerModel.Init(L"Assets/modelData/busyo.cmo");
	//移動状態のロード
	m_busyoAnimeClip[animClip_idle].Load(L"Assets/animData/busyo_idle.tka");
	m_busyoAnimeClip[animClip_idle].SetLoopFlag(true);
	m_busyoAnimeClip[animClip_Walk].Load(L"Assets/animData/busyo_walk.tka");
	m_busyoAnimeClip[animClip_Walk].SetLoopFlag(true);
	//攻撃アニメロード
	m_busyoAnimeClip[animClip_ATK1].Load(L"Assets/animData/busyo_kougeki.tka");
	m_busyoAnimeClip[animClip_ATK2].Load(L"Assets/animData/busyo_kougeki2.tka");
	m_busyoAnimeClip[animClip_ATK3].Load(L"Assets/animData/busyo_kougeki3.tka");
	m_busyoAnimeClip[animClip_ATK4].Load(L"Assets/animData/busyo_kougeki4.tka");
	m_busyoAnimeClip[animClip_ATK5].Load(L"Assets/animData/busyo_kougeki5.tka");
	//ダメージロード
	m_busyoAnimeClip[animClip_SmallDamage].Load(L"Assets/animData/busyo_smalldamage.tka");
	m_busyoAnimeClip[animClip_busyo_dead].Load(L"Assets/animData/busyo_dead.tka");
	//全部一回の再生でいいのでfalseにしとく
	for (int i = animClip_ATK1; i < animClip_num; i++) {
		m_busyoAnimeClip[i].SetLoopFlag(false);
	}
	//アニメーション先モデルとアニメーションの個数を設定。
	m_busyoAnime.Init(
		m_playerModel,
		m_busyoAnimeClip,
		animClip_num
	);

	m_skelton = &m_playerModel.GetSkeleton();
	const wchar_t* bonename[30];

	for (int i = 0; i < 29; i++) {
		bonename[i] = m_skelton->GetBone(i)->GetName();

		if (i == 28)
		{
			bonename[i + 1] = L"end";
		}
	}

	m_busyoAnime.AddAnimationEventListener(	[&](const wchar_t* clipName, const wchar_t* eventName)
	{
			auto m_bone = m_skelton->GetBone(20);
			CVector3 bonepos;
			bonepos.Set(
				m_bone->GetWorldMatrix().m[3][0],
				m_bone->GetWorldMatrix().m[3][1],
				m_bone->GetWorldMatrix().m[3][2]
			);
		//OnAnimationEvent(clipName,eventName);
		m_pl_Wepon = g_goMgr.NewGameObject<Wepon_ghost>("PL_Wepon");
		m_pl_Wepon->SetPosition(bonepos);
		m_pl_Wepon->GhostInit();
		//(void)clipName;
		//MessageBox(NULL, "Attack", "attack", MB_OK);
	}
	);
	ghostInit();
}


Player::~Player()
{
}

void Player::CharaconInit()
{
	//キャラコンの初期化
	m_characon.Init(
		PLAYER_COLLIDER_RADIUS,//コライダーの半径の長さ
		PLAYER_COLLIDER_HEIGHT,//コライダーの高さ
		m_position
	);
}

void Player::Update()
{
	if (!m_deadFrag) {
		//地面ついてる？
		if (m_characon.IsOnGround()) {
			//重力はいらない
			AttackMove();
			gravity_keisuu = 0.1f;
			//ジャンプしてた？
			if (m_Jumpfrag) {
				m_Jumpfrag = false;
			}
		}
		//ステートごとにの処理に後でする。
		if (g_pad->IsTrigger(enButtonA)) {
			if (!m_Jumpfrag) {
				m_moveSpeed.y += JumpPower;
				m_Jumpfrag = true;
				m_animStep = 0;
			}
			gravity_keisuu += 0.1f;
			m_busyoAnime.Play(animClip_idle, 0.5f);
		}
		//ここら辺の処理ではほかに関数を使った方がいいかも
		if (m_damagefrag)
		{
			m_damagefrag = false;
			if (PL_HP != 0) {
				PL_HP--;
			}
			else {
				m_deadFrag = true;
			}
			m_playerState = animClip_SmallDamage;
			m_busyoAnime.Play(animClip_SmallDamage, 0.5f);
		}

		//ダメージアニメーションが終わったら立ち姿に
		if (m_playerState == animClip_SmallDamage
			&& !m_busyoAnime.IsPlaying())
		{
			m_playerState = animClip_idle;
			m_busyoAnime.Play(animClip_idle, 0.5f);
		}
		//プレイヤーが死んでいない時の処理。
		//平面の移動量はアプデごとにリセットする
		m_moveSpeed.x = 0.0f;
		m_moveSpeed.z = 0.0f;
		//入力量を受け取る
		float WideMove = g_pad->GetLStickXF();
		float heightMove = g_pad->GetLStickYF();

		//カメラの前方向と右方向を取得
		m_CameraForward = g_camera3D.GetForword();
		m_CameraRight = g_camera3D.GetRight();
		//Yの情報はいらないので0にし、前と右方向の単位とする。
		m_CameraForward.y = 0.0f;
		m_CameraForward.Normalize();
		m_CameraRight.y = 0.0f;
		m_CameraRight.Normalize();
		m_moveSpeed += m_CameraForward * heightMove * SpeedAmount;
		m_moveSpeed += m_CameraRight * WideMove * SpeedAmount;
		m_moveSpeed.y -= gravity * gravity_keisuu;

		//回転処理
		Turn();
	}
	else {
		//プレイヤーが死んでいる時の処理
		m_moveSpeed = CVector3::Zero();
		m_busyoAnime.Play(animClip_busyo_dead);
	}
	//ワールド行列の更新。
	//m_ghostObject.Release();

	m_position = m_characon.Execute(1.0f / 60.0f, m_moveSpeed);

	CVector3 a = m_position;
	a.y += 80.0f;
	m_ghostObject.SetPosition(a);

	if (m_pl_Wepon != nullptr) {
		m_pl_Wepon->SetPosition(m_position);
	}
	//m_position.Set(CVector3::Zero());
	m_playerModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);

	m_busyoAnime.Update(1.0f / 30.0f);
}
void Player::Draw()
{
	//プレイヤーを描写
	m_playerModel.Draw(
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
}
void Player::Turn()
{
	if (fabsf(m_moveSpeed.x) <= 0.001f    //fabsfは絶対値。m_movespeed.x&m_movespeedzが
		&& fabsf(m_moveSpeed.z) <= 0.001f) {//0.001以下の時には何もしない。
		return;
	}
	else {
		float angle = atan2(m_moveSpeed.x, m_moveSpeed.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
	}
}
void Player::AttackMove()
{
	//補間時間
	float InterpolationTime = 0.5;
	if (g_pad->IsTrigger(enButtonX)&&m_playTimer>10.0f) {
		int i = 0;
		//判定します。
		//ストラテジーパターン予備軍
		switch (m_animStep)
		{
		case 0:
			if (!m_underAttack)
			{
				m_underAttack = true;
			}
			m_busyoAnime.Play(animClip_ATK1, InterpolationTime);
			m_animStep++;
			break;
		case 1:
			m_busyoAnime.Play(animClip_ATK2, InterpolationTime);
			m_animStep++;
			break;
		case 2:
			m_busyoAnime.Play(animClip_ATK3, InterpolationTime);
			m_animStep++;
			break;
		case 3:
			m_busyoAnime.Play(animClip_ATK4, InterpolationTime);
			m_animStep++;
			break;
		case 4:
			m_busyoAnime.Play(animClip_ATK5, InterpolationTime);
			m_animStep++;
			break;
		}
	}
	m_playTimer++;
	if (m_animStep != 0) {
		if (m_animStep != m_oldAnimStep) {
			m_playTimer = 0;
			m_oldAnimStep = m_animStep;
		}
		if (m_playTimer >= 20) {
			//一定の時間が過ぎたらアニメステート関係を初期化
			if (m_underAttack) {
				m_underAttack = false;
			}
			m_animStep = 0;
			m_oldAnimStep = 0;
			m_playTimer = 0;
			m_busyoAnime.Play(animClip_idle, InterpolationTime*2.0f);
		}
	}
}



int Player::RequestEnemyData(CVector3 pos,Enemy* enemy)
{
	for (int i = 0; i < kakoi_max; i++)
	{
		
		//一番最初にエネミーの空いている所に情報を入れる
		if (m_enemydata[i].position.y == NULL) {
			m_enemydata[i].position = pos;
			m_enemydata[i].enemy = enemy;
			return i;
		}
		else if (m_enemydata[i].enemy == enemy) {
			return i;
		}

		if (m_enemydata[i].position.y != NULL) {
			CVector3 kyori = m_enemydata[i].position - pos;
			if (kyori.LengthSq() > posClearRange) {
				m_enemydata[i].enemy = NULL;
				m_enemydata[i].position = CVector3{NULL,NULL,NULL};
				return -1;
			}
		}
	}
	//nullでないかつ
	//距離が離れていたら
	//posをヌルにして
	//-1へ
	//こっちに行くと通常通り
	return -1;
}

void Player::ghostInit()
{
	CVector3 PlayerScale = { 50.0f,150.0f,50.0f };
	m_ghostObject.CreateBox(
		m_position,
		m_rotation,
		PlayerScale
	);
}