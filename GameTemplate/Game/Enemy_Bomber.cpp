#include "stdafx.h"
#include "gameObject/ysGameObjectManager.h"
#include "Enemy_Bomber.h"
#include "Wepon_ghost.h"
#include "bom.h"
const float INTERPOLATION_TIME = 0.2f;			//アニメーションの補間時間
const float BOMBER_SPEED = 5.0f;
const float HARF_RENGE = 1400.0f;
const float SOCIAL_DISTANCE = 700.0f;			//プレイヤーとの間合いです。
const float ESCAPE_SPEED = 200.0f;				//プレイヤーから逃げるスピード
const float GRAVITY = -20.0f;					//重力

const float MODEL_RADIUS = 60.0f;					//敵のモデルの半径。コリジョンを作るために使用
const float MODEL_HIGHT = 100.0f;					//敵のモデルの高さ。コリジョンを作るために使用
const float EFFECTPOS_Y = 100.0f;					//エフェクトを出す位置の高さを調整。

const float SOUND_VOL = 0.55f;					//鳴らす音のおおきさ。数値が大きいほど音も大きくなる
Enemy_Bomber::Enemy_Bomber()
{
	//ボム敵のモデルはasigaruのモデルに槍を取っ払ったものなので
	//倒れる時とかのアニメはasigaruのアニメを流用します。
	m_model.Init(L"Assets/modelData/asigaru_taicho.cmo");
	m_model.SetShadowCaster(false);
	m_rotation = CQuaternion::Identity();
	m_scale = CVector3::One();
	m_bomAnimeClip[Asigaru_attack].Load(L"Assets/animData/asigaru_nage.tka");
	m_bomAnimeClip[Asigaru_escape].Load(L"Assets/animData/asigaru_tikazuki.tka");
	m_bomAnimeClip[Asigaru_damage].Load(L"Assets/animData/asigaru_damage.tka");
	m_bomAnimeClip[Asigaru_dead].Load(L"Assets/animData/asigaru_dead.tka");
	m_bomAnimeClip[Asigaru_zyosou].Load(L"Assets/animData/asigaru_zyosou.tka");
	m_bomAnimeClip[Asigaru_Idle].Load(L"Assets/animData/asigaru_bomIdle.tka");
	m_bomAnimeClip[Asigaru_attack].SetLoopFlag(false);
	m_bomAnimeClip[Asigaru_escape].SetLoopFlag(false);
	m_bomAnimeClip[Asigaru_damage].SetLoopFlag(false);
	m_bomAnimeClip[Asigaru_dead].SetLoopFlag(false);
	m_bomAnimeClip[Asigaru_zyosou].SetLoopFlag(false);
	m_bomAnimeClip[Asigaru_Idle].SetLoopFlag(false);
	//モデルとアニメデータを初期化
	m_bomberAnime.Init(
		m_model,
		m_bomAnimeClip,
		Asigaru_anim_num//テスト
	);
	m_bomberAnime.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName) {
		(void)clipName;
			m_bom = g_goMgr.NewGameObject<Bom>("bom");
			m_bom->SetPosition(m_position);
			m_bom->SetPlayerInfo(m_player);
			m_bom->SetInpactPosition(m_player->GetPosition());
		}
	);
}

Enemy_Bomber::~Enemy_Bomber()
{
	//if (m_bom != nullptr) {
	//	m_bom = nullptr;
	//	DeleteGO(m_bom);
	//}
	g_goMgr.DeleteGOObject(this);
}

void Enemy_Bomber::Draw()
{
	//モデルの描画
	m_model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void Enemy_Bomber::Update()
{
	//キャラクターコントローラーなかったら作る
	if (!m_isCharaconUse) {
		m_isCharaconUse = true;
		CharaconInit();
	}
	//m_moveSpeed = m_player->GetPosition() - m_position;
	//m_moveSpeed.Normalize();
	//m_moveSpeed *= bomberSpeed;
	if (!m_bomberAnime.IsPlaying()) {
		Statekanri();
		Animekanri();
		Move();
		Turn();
	}
	if (m_isDeadfrag) {
		DeathMove();
	}

	//武器のゴーストが自分たちに当たったら、死んだという信号を立てる
	QueryGOs<Wepon_ghost>("PL_Wepon", [&](Wepon_ghost* wepon) {
		PhysicsGhostObject* ghostobject = wepon->GetGhostObject();
		g_physics.ContactTest(m_characon, [&](const btCollisionObject& contactObject) {
			if (ghostobject->IsSelf(contactObject) == true) {
				//通っているのは確認完了
				m_isDeadfrag = true;
				//enemy用にも
				ThisDeath();
				CSoundSource* m_se2 = new CSoundSource;
				m_se2->Init(L"Assets/sound/slash1.wav");
				m_se2->Play(false);
				m_se2->SetVolume(SOUND_VOL);
				//エフェクトも出す。
				g_Effect.m_playEffectHandle = g_Effect.m_effekseerManager->Play(
					g_Effect.m_sampleEffect,
					m_position.x,
					m_position.y + EFFECTPOS_Y,
					m_position.z
				);
			}
			});
		return true;
		});

	if (!m_characon.IsOnGround()) {
		m_moveSpeed.y += grabity;
	}
	m_position = m_characon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_bomberAnime.Update(GameTime().GetFrameDeltaTime());
}

void Enemy_Bomber::Statekanri()
{
	// ボマーのアニメーションが終わった時のbomberの状態によって、
	// ステートを変える処理を実装
	CVector3 distans = m_position - m_player->GetPosition();
	if (distans.LengthSq() < HARF_RENGE * HARF_RENGE) {
		if (m_state != Asigaru_attack) {
			m_state = Asigaru_attack;
		}
		else if(!m_bomberAnime.IsPlaying()){
			m_state = Asigaru_escape;
		}
	}
	else {
		m_state = Asigaru_Idle;
	}
	if (m_isDeadfrag) {
		m_state = Asigaru_dead;
	}
}

void Enemy_Bomber::Animekanri()
{
	//渡されたステートからどんなアニメーションを呼ぶかをこの関数内でやる象
	//今の実装ではUpdateに1行足しただけで終わるけど、後から変更される可能性大なので
	//関数にしました。

	m_bomberAnime.Play(m_state, INTERPOLATION_TIME);
}

//回転
void Enemy_Bomber::Turn()
{
	auto Rot = CMatrix::Identity();
	Rot.MakeRotationFromQuaternion(m_rotation);

	if (fabsf(m_moveSpeed.x) <= 0.001f    //fabsfは絶対値。m_movespeed.x&m_movespeedzが
		&& fabsf(m_moveSpeed.z) <= 0.001f) {//0.001以下の時には何もしない。
		return;
	}
	else {
		float angle = atan2(m_moveSpeed.x, m_moveSpeed.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
	}
}

//爆弾を投げた後の動き
void Enemy_Bomber::EscapeMove()
{
	CVector3 distance = m_player->GetPosition() - m_position;
	if (distance.Length() < SOCIAL_DISTANCE) {
		CVector3 hikigimi = m_position - m_player->GetPosition();
		hikigimi.y = 0.0f;
		hikigimi.Normalize();
		hikigimi *= ESCAPE_SPEED;
		m_moveSpeed += hikigimi;
	}
}
//攻撃方法、Turn()をつかう
void Enemy_Bomber::attackMove()
{
	m_moveSpeed = m_player->GetPosition() - m_position;
	Turn();
	m_moveSpeed = CVector3::Zero();
}

//死んでしまった時の動き
void Enemy_Bomber::DeathMove()
{
	if (m_isDeadfrag) {
		m_state = Asigaru_dead;
		m_bomberAnime.Play(m_state, INTERPOLATION_TIME);
		if (!m_bomberAnime.IsPlaying()) {
			DeleteGO(this);
		}
	}
}

//ステートで決める
void Enemy_Bomber::Move()
{
	if (m_state == Asigaru_attack) {
		attackMove();
	}
	if (m_state == Asigaru_escape) {
		EscapeMove();
	}
}

//呼ばれるとキャラコンの機能が入る。
void Enemy_Bomber::CharaconInit()
{
	//キャラコンの初期化
	m_characon.Init(
		MODEL_RADIUS, //半径
		MODEL_HIGHT,//高さ
		m_position//位置
	);
}