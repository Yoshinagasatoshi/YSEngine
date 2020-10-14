#include "stdafx.h"
#include "gameObject/ysGameObjectManager.h"
#include "Wepon_ghost.h"
#include "Enemy_Busyo.h"
#include "GameClear.h"
#include "Game.h"
#include "InGameSoundDirector.h"
#include "Fade.h"
#include "InGameSoundDirector.h"
const float power = 250.0f;
//ボスの体力。ここで設定する。
const float InitHP = 1;
Enemy_Busyo::Enemy_Busyo()
{
	//HPを設定
	m_HP = InitHP;
	m_model.Init(L"Assets/modelData/enemy_busyo.cmo");

	//normalmap
	DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(),
		L"Assets/modelData/Ch24_1001_Normal.dds",		//ロードするテクスチャのパス
		0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
		0,
		0,
		false,
		nullptr,
		&m_normalMapSRV	//作成されたSRVのアドレスの格納先
	);

	//specmap
	DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(),
		L"Assets/modelData/Ch24_1001_Specular.dds", 
		0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
		0, 
		0,
		false,
		nullptr,
		&m_specMapSRV
	);

	m_rotation = CQuaternion::Identity();
	m_scale = CVector3::One();
	//m_animClip[ATK].Load(L"Assets/animData/enemy_Busyo_Attack.tka");
	//アニメーションデータをロード
	m_animClip[ATK].Load(L"Assets/animData/atodekesuKick.tka");
	m_animClip[IDL].Load(L"Assets/animData/enemy_Busyo_Idle.tka");
	m_animClip[MOVE].Load(L"Assets/animData/enemy_Busyo_Inflate.tka");
	m_animClip[DAMAGE].Load(L"Assets/animData/enemy_Busyo_Damage.tka");
	m_animClip[DEATH].Load(L"Assets/animData/enemy_Busyo_Death.tka");
	m_animClip[FIGHTING].Load(L"Assets/animData/enemy_Busyo_Fighting_Pose.tka");
	m_animClip[FIGHTING_KICK].Load(L"Assets/animData/enemy_Busyo_kkkick.tka");
	//m_animClip[FIGHTING_KICK].Load(L"Assets/animData/enemyBusyo_newKick.tka");
	m_animClip[FIGHTING_LONG].Load(L"Assets/animData/enemy_Busyo_LongAttack.tka");
	m_animClip[LEFT_STEP].Load(L"Assets/animData/enemy_Busyo_Leftstep.tka");

	//モデルにノーマルマップとスペキュラを適用
	m_model.SetNormalMap(m_normalMapSRV);

	m_model.SetSpecularMap(m_specMapSRV);

	//アニメーションするモデルとアニメーションデータを紐づける
	m_enemy_BusyoAnime.Init(
		m_model,
		m_animClip,
		AnimationClip_Num
	);
	//アニメーションがループするかのフラグを設定
	m_animClip[ATK].SetLoopFlag(false);
	m_animClip[MOVE].SetLoopFlag(false);
	m_animClip[IDL].SetLoopFlag(true);
	m_animClip[DAMAGE].SetLoopFlag(false);
	m_animClip[DEATH].SetLoopFlag(false);
	m_animClip[FIGHTING].SetLoopFlag(true);
	m_animClip[FIGHTING_KICK].SetLoopFlag(false);
	m_animClip[FIGHTING_LONG].SetLoopFlag(false);
	m_animClip[LEFT_STEP].SetLoopFlag(false);

	//アニメーションイベントが呼ばれたときの処理
	//攻撃判定を出すようにしている。
	m_enemy_BusyoAnime.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName) {
		(void)clipName;
		m_en_Wepon = g_goMgr.NewGameObject<Wepon_ghost>("EN_Wepon");
		m_en_Wepon->SetPosition(m_position);
		m_en_Wepon->SetPlayerInfo(m_player);
		m_en_Wepon->GhostInit();
		//音を出す。
		InGameSoundDirector::GetInstans().RingSE_Kick();
		}
	);
}

Enemy_Busyo::~Enemy_Busyo()
{

}

void Enemy_Busyo::Update()
{
	//1フレーム前の位置を更新
	m_lastPosition = m_position;
	//キャラコンが入っていなかったら入れる。
	if (!m_charaConUse) {
		CharaconInit();
		m_charaConUse = true;
	}
	//m_stateジャッジ。この中でm_stateが割り振られる。
	StateJudge();

	switch (m_state)
	{
	case ATK:
		m_player->RequestEnemyData(m_position, this);
		AttackMove();
		break;
	case MOVE:
		NormalMove();
		break;
	case IDL:
		IdleMove();
		break;
	case DAMAGE:
		ThisDelete();
	case DAMAGE_AFTER:
		DamageAfter();
	}
	//ダメージ処理
	ThisDamage();

	//ワールド座標の更新　こっちのskeletonUpdateをいじる
	auto footStep = m_enemy_BusyoAnime.Update(1.0f / 30.0f);//ローカル座標の更新　こっちはいじらない
	//if (m_busyoState == BusyoAttack) {
	//攻撃中はフットステップの移動量を加算する。
	CMatrix mBias = CMatrix::Identity();
	mBias.MakeRotationX(CMath::PI * -0.5f);
	CMatrix rotMatrix;
	//回転行列を作成する。
	rotMatrix.MakeRotationFromQuaternion(m_rotation);
	rotMatrix.Mul(mBias, rotMatrix);
	rotMatrix.Mul(footStep);
	footStep *= 600.0f;
	m_moveSpeed += footStep;

	m_position = m_characon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	//ワールド行列の更新。
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}

void Enemy_Busyo::CharaconInit()
{
	//キャラコンの初期化
	m_characon.Init(
		60.0f, //半径
		100.0f,//高さ
		m_position//位置
	);
}

void Enemy_Busyo::Draw()
{
	m_model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

// プレイヤーとの距離で呼ぶ関数を変えている処理
void Enemy_Busyo::StateJudge()
{
	
	distance = m_player->GetPosition() - m_position;

	//ダメージを受けた後でなければ
	if (m_state != DAMAGE_AFTER) {
		//特殊な攻撃の「飛び蹴り中」でなければ
		if (!m_isFightingKick) {
			if (distance.LengthSq() > BattleRange
				&& distance.LengthSq() < VigilanceRange) {
				m_state = MOVE;
			}
			if (distance.LengthSq() > VigilanceRange) {
				m_state = IDL;
			}
		}
			if (distance.LengthSq() < BattleRange) {
				m_state = ATK;
			}
		//ダメージを受けている時は他ののステートよりも優先
		if (m_isDeadfrag) {
			m_state = DAMAGE;
		}
	}
}



//攻撃範囲に来たらここに入る。
void Enemy_Busyo::AttackMove()
{
	m_frameTimer += 1.0f;

	//攻撃中の動き
	m_moveSpeed = CVector3::Zero();
	//ワールド座標の更新
	m_moveSpeed.y += grabity * 10.0f;
	if (m_characon.IsOnGround()) {
		m_moveSpeed.y = 0.0f;
	}
	//攻撃の感覚を決める
	AttackframeNum();
	// 事前に動いていた距離の量により
	// すぐに回転蹴りするか、立ち止まり
	// 待って攻撃するかを変えている
	// 2000が今の走行距離条件分岐です。
	if (m_mileage > 200.0f) {
		if (!m_isFight) {
			m_isFight = true;
		}
		//アニメーションイベントがある
		//アニメーションが流れたらダメージを食らうはず
		//Fightng_Kickはかなり特殊。プレイヤーを吹き飛ばしたい。
		m_isFightingKick = true;
		m_enemy_BusyoAnime.Play(FIGHTING_KICK, 0.5f);
		if (!m_enemy_BusyoAnime.IsPlaying())
		{
			m_frameTimer = 0;
			m_enemy_BusyoAnime.Play(MOVE, 0.2f);
			m_isFight = false;
			m_isFightingKick = false;
			m_mileage = 0.0f;
		}
	}
	if (m_frameTimer > m_attackFrameNum)
	{

			m_isATKMode = true;
			if (m_mileage < 200.0f) {
				m_enemy_BusyoAnime.Play(ATK, 0.2f);
				if (!m_enemy_BusyoAnime.IsPlaying())
				{
					m_frameTimer = 0;
					m_enemy_BusyoAnime.Play(MOVE, 0.2f);
					m_isFight = false;
					m_mileage = 0.0f;
				}
			}
	}

	if (!m_isFight) {
		m_enemy_BusyoAnime.Play(FIGHTING, 0.1f);
	}

	m_position = m_characon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
}

//索敵範囲に来たらここに入る
void Enemy_Busyo::NormalMove()
{
	if (distance.LengthSq() > 200.0f * 200.0f) {
		CVector3 direction = distance;
		direction.y = 0.0f;
		direction.Normalize();
		m_moveSpeed = direction * power;
		//ワールド座標の更新
		m_moveSpeed.y += grabity;
		if (m_characon.IsOnGround()) {
			m_moveSpeed.y = 0.0f;
		}
		float angle = atan2(distance.x, distance.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
		m_enemy_BusyoAnime.Play(MOVE, 0.2f);
		//アニメーションイベントのために歩きアニメをループ再生からワンショット再生に変更。
		//それをごまかすために、アニメーション再生が終わったら1フレームだけ別のアニメを入れる。
		if (!m_enemy_BusyoAnime.IsPlaying()) {
			m_enemy_BusyoAnime.Play(IDL, 0.1f);
		}
	}
	else {
		m_moveSpeed = CVector3::Zero();
		m_enemy_BusyoAnime.Play(IDL, 0.1f);
	}
	m_position = m_characon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	//ここで計算を入れる
	CVector3 vec = m_position - m_lastPosition;
	m_mileage += vec.Length();
}

//距離が遠すぎたらここに入る。
void Enemy_Busyo::IdleMove()
{
	m_moveSpeed = CVector3::Zero();
	//ワールド座標の更新
	m_moveSpeed.y += grabity;
	if (m_characon.IsOnGround()) {
		m_moveSpeed.y = 0.0f;
	}

	m_enemy_BusyoAnime.Play(IDL, 0.2f);
	m_position = m_characon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
}

void Enemy_Busyo::ThisDamage()
{
	//武器のゴーストが自分たちに当たったら、ダメージを食らったと知らせるようにする
	QueryGOs<Wepon_ghost>("PL_Wepon", [&](Wepon_ghost* wepon) {
		PhysicsGhostObject* ghostobject = wepon->GetGhostObject();
		g_physics.ContactTest(m_characon, [&](const btCollisionObject& contactObject) {
			if (ghostobject->IsSelf(contactObject) == true) {
				//通っているのは確認完了
				m_isDeadfrag = true;
				//エフェクトも出す。
				g_Effect.m_playEffectHandle = g_Effect.m_effekseerManager->Play(
					g_Effect.m_sampleEffect,
					m_position.x, m_position.y + 100.0f,
					m_position.z
				);

			}
			});
		return true;
		});
}

void Enemy_Busyo::ThisDelete()
{
	//体力はまだ残っているかどうかj
	if (m_HP != 0) {
		InGameSoundDirector::GetInstans().RingSE_Slash();
		m_HP--;
		m_enemy_BusyoAnime.Play(DAMAGE, 0.2f);
		m_state = DAMAGE_AFTER;
		m_moveSpeed = CVector3::Zero();
	}
	//なくなっていたらこちらを通る
	else {
		//enemy用にも
		ThisDeath();
		m_state = DAMAGE;
		m_moveSpeed = CVector3::Zero();
		
		//プレイヤーに無敵をつける。
		//プレイヤーがボスを倒した後に死ぬと
		//ゲームクリアとゲームオーバーが一緒に出てしまうため。
		m_player->SetPlayerMuTeki();

		m_enemy_BusyoAnime.Play(DEATH, 0.1f);
		if (!m_isDestroyed && !m_enemy_BusyoAnime.IsPlaying()) {
			{
				Fade::Getinstance().StartFadeIn();
			}
			if (!Fade::Getinstance().IsFade()) {
				g_goMgr.ResetCount();
				InGameSoundDirector::GetInstans().UpdateOff();
				InGameSoundDirector::GetInstans().SoundRelease();

				//倒せばokという状態に
				g_goMgr.NewGameObject<GameClear>("GameClear");
				m_game->GameDelete();
				m_isDestroyed = true;
			}
		}

	}
}

void Enemy_Busyo::DamageAfter()
{
	//アニメーションが流し終わったらIDLに切り替える。
	if (!m_enemy_BusyoAnime.IsPlaying()) {
		//切り替え
		m_state = IDL;
		//m_enemy_BusyoAnime.Play(IDL, 0.2f);
		//体力がゼロじゃなければダメージが入るようにもする、
		if (m_HP != 0) {
			m_isDeadfrag = false;
		}
	}
}