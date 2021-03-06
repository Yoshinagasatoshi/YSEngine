#include "stdafx.h"
#include "Player.h"
#include "Player_target.h"
#include <iostream>
#include "gameObject/ysGameObjectManager.h"
#include "Enemy.h"
#include "Enemy_asigaru.h"
#include "Wepon_ghost.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
#include "InGameSoundDirector.h"
#include "GameOver.h"
#include "Game.h"
#include "Fade.h"
#include "graphics\shadow\ShadowMap.h"

const float POSCLEAR_RANGE = 600.0f * 600.0f;	//クリア判定を行う範囲。ゲームクリアではない
const float PLAYER_COLLIDER_HEIGHT = 100.0f;	//プレイヤーのカプセルコライダーの高さ。
const float PLAYER_COLLIDER_RADIUS = 60.0f;		//プレイヤーのカプセルコライダーの半径。

const float KAKOI_MAX = 5;						//囲いの数
const float SPEEDAMOUNT = 1500.0f;				//平面の移動量
const float GRAVITY = 200.0f;					//重力
const float GRAVITY_START = 0.1f;				//重力加速値_初期値
const float GRAVITY_ACCELE = 0.1f;				//重力加速値
const float JUMPPOWER = 1000.0f;				//プレイヤーの飛ぶ力
const float JUMPATKPOWER = 350.0f;				//ジャンプ義理の強さ
const float STANDARDPOWER = 200.0f;				//プレイヤーの敵吹き飛ばし力
const float LIMIT = 2.0f;						//重力係数の上限
const int	TIMER_ZERO = 0;						//計測時間が0になる。
const int	ONEBROCK = 24;						//プレイヤが敵から受ける定数ダメージ。ボスからの攻撃とか威力が高い奴には*2とかすると思う
const float GHOST_Y_HOSEI = 70.0f;				//ゴーストのYにどれくらいの補正をかけたか
const float MUSOU_SYOUHI = 450.0f;				//無双奥義を打つ時に消費する数値
const float ANIMFOOT_BAIRITU = 32.0f;			//アニメのフットステップに書ける倍率
const float BIAS = -0.5f;						//アニメーションの移動量を調整するバイアス
const float PLAYER_TIMER_RIMIT = 0.5f;			//アニメーションの限界
const float MUSOU_GAUGE_ADD = 50.0f;			//攻撃を受けたときに加算されるゲージ量
const float PLAYER_TIMER = 0.1f;				//プレイヤータイマーの条件式に使う
const float EFFECT_POSITION = 100.0f;			//エフェクトの位置を少し高くするための数値

const float X_ZERO = 0.0f;						//Xの数値を0にする
const float Y_ZERO = 0.0f;						//Yの数値を0にする
const float Z_ZERO = 0.0f;						//Zの数値を0にする

const int	PL_DEADHP = 0;						//プレイヤーのHPがゼロである。
const int	ENEMY_NOT_NUM = 0;					//エネミーはゼロですという事を表す
const int	ANIM_STEP_RESET = 0;				//アニメーションステップのリセット
const int	ANIM_STEP_NO_USE = 0;				//アニメーションイベントが動いていない時に使われる
const int	ANIMEVENTBORN = 20;					//アニメーションイベント情報が組み込まれているボーンの数字

const float RELEASE_TIME = 0.4f;				//戦闘中のアニメーションイベントの解放条件。この数値より上なら開放

const float SE_RUN_SPEED = 300.0f;				//移動速度がこの数値以上なら走る音を鳴らす

const float CONSTANT_SPEED = 1.0f;				//一定のスピード。走るアニメーションの条件分岐に使用

const int	PLAYER_BORN_NUM = 30;				//プレイヤーのボーンの数
//アニメーションの補間時間・小
const float INTERPOLATIONTIME_S = 0.1f;		
//アニメーションの補間時間・中
const float INTERPOLATIONTIME_M = 0.2f;
//アニメーションの補間時間・大
const float INTERPOLATIONTIME_L = 0.5f;			

//特別なアニメーションの補間時間
const float INTERPOLATIONTIME_SP = 0.4;
Player::Player()
{
	testID = rand();
	//無敵状態を解除
	m_muteki = false;

	//m_bgm.Init(L"Assets/sound/Chanbara.wav");
	//m_bgm.Play(true);
	////seチェックのために、大分下げる
	//m_bgm.SetVolume(1.0f);
	CharaconInit();
	m_pl_target = g_goMgr.NewGameObject<Player_target>("PLT");
	m_pl_target->SetPlayerInfo(this);
	//cmoファイルの読み込み。
	m_playerModel.Init(L"Assets/modelData/busyo.cmo");
	DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(),
		L"Assets/modelData/_Users_GC1831_Desktop_YSEngine_YSEngine_GameTemplate_Game_Assets_modelData_BUSYO__HUKU.png.dds",		//ロードするテクスチャのパス
		0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
		0,
		0,
		false,
		nullptr,
		&m_normalMapSRV	//作成されたSRVのアドレスの格納先
	);
	//移動状態のロード
	m_busyoAnimeClip[animClip_idle].Load(L"Assets/animData/busyo_idle.tka");
	m_busyoAnimeClip[animClip_idle].SetLoopFlag(true);
	m_busyoAnimeClip[animClip_Walk].Load(L"Assets/animData/busyo_walk.tka");
	m_busyoAnimeClip[animClip_Walk].SetLoopFlag(true);
	m_busyoAnimeClip[animClip_jump].Load(L"Assets/animData/busyo_jump.tka");
	m_busyoAnimeClip[animClip_jump].SetLoopFlag(false);
	//攻撃アニメロード
	m_busyoAnimeClip[animClip_ATK1].Load(L"Assets/animData/busyo_kougeki.tka");
	m_busyoAnimeClip[animClip_ATK2].Load(L"Assets/animData/busyo_kougeki2.tka");
	m_busyoAnimeClip[animClip_ATK3].Load(L"Assets/animData/busyo_kougeki3.tka");
	m_busyoAnimeClip[animClip_ATK4].Load(L"Assets/animData/busyo_kougeki4.tka");
	m_busyoAnimeClip[animClip_ATK5].Load(L"Assets/animData/busyo_kougeki5.tka");
	m_busyoAnimeClip[animClip_XATK].Load(L"Assets/animData/busyo_kaitengiri.tka");
	m_busyoAnimeClip[animClip_JUMP_ATK].Load(L"Assets/animData/busyo_jump_kougeki.tka");
	m_busyoAnimeClip[animClip_JUMP_X_ATK1].Load(L"Assets/animData/busyo_jump_X.tka");
	m_busyoAnimeClip[animClip_JUMP_X_ATK2].Load(L"Assets/animData/busyo_jump_X2.tka");

	//ダメージロード
	m_busyoAnimeClip[animClip_SmallDamage].Load(L"Assets/animData/busyo_smalldamage.tka");
	m_busyoAnimeClip[animClip_busyo_dead].Load(L"Assets/animData/busyo_dead.tka");
	//全部一回の再生でいいのでfalseに設定
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
	//このコードはボーンの配列を確認するために書いているコード。直接ゲームには関わってない。
	const wchar_t* bonename[PLAYER_BORN_NUM];

	for (int i = 0; i < PLAYER_BORN_NUM - 1; i++) {
		bonename[i] = m_skelton->GetBone(i)->GetName();

		if (i == PLAYER_BORN_NUM - 2)
		{
			bonename[i + 1] = L"end";
		}
	}

	m_busyoAnime.AddAnimationEventListener(	[&](const wchar_t* clipName, const wchar_t* eventName)
	{
		auto m_bone = m_skelton->GetBone(ANIMEVENTBORN); //二十番目のボーンを取得。引数のボーンにアニメーションイベント情報が組み込まれている。
		CVector3 bonepos;
		//y成分を除いたボーンをセット
		bonepos.Set(
			m_bone->GetWorldMatrix().m[3][0],
			0,
			m_bone->GetWorldMatrix().m[3][2]
		);
		//OnAnimationEvent(clipName,eventName);

		//ボーンのposとプレイヤーのposを足した場所
		m_calcPos = m_position + bonepos;
		//ghostが半分埋まっていたので少し上に合わせる。
		m_calcPos.y += GHOST_Y_HOSEI;

		m_pl_Wepon = g_goMgr.NewGameObject<Wepon_ghost>("PL_Wepon");
		m_pl_Wepon->SetPosition(m_calcPos);
		m_pl_Wepon->SetPlayerInfo(this);
		m_pl_Wepon->Playercall();
		m_pl_Wepon->GhostInit();

		//サウンドディレクターに一任する
		InGameSoundDirector::GetInstans().RingSE_Swing();

		//if (!m_se.IsPlaying()) {
		//	m_se.Init(L"Assets/sound/swing.wav");
		//	m_se.Play(false);
		//	m_se.SetVolume(3.0f);//試しにでかくしている。後で調整
		//}
		//else {
		//	if (!m_se2.IsPlaying()) {
		//		m_se2.Init(L"Assets/sound/swing.wav");
		//		m_se2.Play(false);
		//		m_se2.SetVolume(3.0f);//試しにでかくしている。後で調整
		//	}
		//}
	}
	);

}


Player::~Player()
{
	//g_goMgr.DeleteGOObject(this);
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
	if (m_deadFrag) {
		m_busyoState = BusyoDead;
	}
	if (m_busyoState != BusyoDead) {
		//ゴースト当たってる？
		ThisDamage();
		//地面ついてる？
		if (m_characon.IsOnGround()) {
			//攻撃するときのモーション
			AttackMove();
			//重力の重みを軽くする
			m_gravity_keisuu = GRAVITY_START;
			//ジャンプしてた？
			if (m_Jumpfrag) {
				m_Jumpfrag = false;
				m_jumpAttackfrag = false;
				//更に空中でYボタン押してた？
				if (m_busyoState == BusyoAttack_Y)
				{
					//着地時にダメージ判定を行うアニメーションを流す
					m_busyoAnime.Play(animClip_JUMP_X_ATK1, INTERPOLATIONTIME_S);
				}
			}
		}
		//ステートごとにの処理に後でする。
		if (m_busyoState != BusyoAttack) {
			if (g_pad->IsTrigger(enButtonA)) {
				if (!m_Jumpfrag) {
					m_moveSpeed.y += JUMPPOWER;
					m_Jumpfrag = true;
					m_animStep = ANIM_STEP_RESET;
					//攻撃モーション中にジャンプするとプレイヤーが動かなくなる時があったので追加
					m_underAttack = false;
				}
			}
			m_gravity_keisuu += GRAVITY_ACCELE;
			if (m_gravity_keisuu > LIMIT) {
				m_gravity_keisuu = LIMIT;
			}
			//ジャンプ状態じゃなければ移動速度によってアニメーションを変える。
			if (!m_Jumpfrag) {
				if (m_moveSpeed.Length() > SE_RUN_SPEED) {
					m_busyoAnime.Play(animClip_Walk, INTERPOLATIONTIME_S);
				}
				else {
					m_busyoAnime.Play(animClip_idle, INTERPOLATIONTIME_S);
				}
			}
			else {
				JumpAttackMove();
			}
		}
		//ここら辺の処理ではほかに関数を使った方がいいかもしれません
		if (m_damagefrag)
		{
			if (!m_muteki) {
				m_damagefrag = false;
				if (m_PL_HP != ONEBROCK) {
					m_PL_HP -= ONEBROCK;
					g_goMgr.AddMusouGauge(MUSOU_GAUGE_ADD);//引数は足される無双奥義のゲージ量
					//g_goMgr.HitStopOn();
				}
				else {
					m_PL_HP = PL_DEADHP;
					m_deadFrag = true;
				}
			}
			m_animState = animClip_SmallDamage;
			m_busyoAnime.Play(animClip_SmallDamage,INTERPOLATIONTIME_L);
		}

		//ダメージアニメーションが終わったら立ち姿に
		if (!m_busyoAnime.IsPlaying())
		{
			m_animState = animClip_idle;
			m_busyoState = BusyoNormal;
			//if (m_underAttack) {
			//	m_underAttack = false;
			//}
			if (!m_Jumpfrag) {
				m_busyoAnime.Play(animClip_idle,INTERPOLATIONTIME_M);
			}
		}
		//移動処理
		Move();
		//haha
		//回転処理
		Turn();

	}
	else {
		DeadPlayer();
	}
	Execute();
	/// <summary>
	/// デバック用コマンド。後で消す。
	/// </summary>
	//if (g_pad->IsTrigger(enButtonLeft)) {
	//	m_busyoState = BusyoDead;
	//}

	g_graphicsEngine->GetShadowMap()->UpdateFromLightTarget(m_position + CVector3::One() * 300.0f,m_position);
}

void Player::Move()
{
	//プレイヤーが死んでいない時の処理。
	//平面の移動量はアプデごとにリセットする。
	m_moveSpeed.x = X_ZERO;
	m_moveSpeed.z = Z_ZERO;
	if (m_busyoState != BusyoAttack) {
		//入力量を受け取る
		WideMoveL = g_pad->GetLStickXF();
		heightMoveL = g_pad->GetLStickYF();

		//カメラの前方向と右方向を取得
		m_CameraForward = g_camera3D.GetForword();
		m_CameraRight = g_camera3D.GetRight();
		//Yの情報はいらないので0にし、前と右方向の単位とする。
		m_CameraForward.y = Y_ZERO;
		m_CameraForward.Normalize();
		m_CameraRight.y = Y_ZERO;
		m_CameraRight.Normalize();
		//攻撃中は自由に動かない時にする。
		//m_busyoState = BusyoAttack;
		if (!m_underAttack) {
			m_moveSpeed += m_CameraForward * heightMoveL * SPEEDAMOUNT;
			m_moveSpeed += m_CameraRight * WideMoveL * SPEEDAMOUNT;
		}
	}
	//一定の速さで進んでいるなら走るSEを追加
	if (m_moveSpeed.Length() > CONSTANT_SPEED
		&&!m_Jumpfrag) {
		InGameSoundDirector::GetInstans().RingSE_Run();
	}
	m_moveSpeed.y -= GRAVITY * m_gravity_keisuu;
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
	if(m_busyoState != BusyoAttack) {
		if (fabsf(m_moveSpeed.x) <= 0.001f    //fabsfは絶対値。m_movespeed.x&m_movespeedzが
			&& fabsf(m_moveSpeed.z) <= 0.001f) {//0.001以下の時には何もしない。
			return;
		}
		else {
			float angle = atan2(m_moveSpeed.x, m_moveSpeed.z);
			m_rotation.SetRotation(CVector3::AxisY(), angle);
		}
	}
	else {
		//攻撃補正をかける処理
		if (m_pl_target->GetHosei()) {
			CVector3 kaiten = m_pl_target->GetDistans();
			float angle = atan2(-kaiten.x, -kaiten.z);
			m_rotation.SetRotation(CVector3::AxisY(), angle);
		}
	}
}

void Player::AttackMove()
{
	if (g_pad->IsTrigger(enButtonX)&&m_playTimer> PLAYER_TIMER) {
		
	//m_busyoState = BusyoAttack;
		if (!m_underAttack)
		{
			m_underAttack = true;
		}
		m_busyoState = BusyoAttack;
		//補正をかけるべきなのかを調べます。
		bool hosei = m_pl_target->GetHosei();
		
		//判定します。
		//ストラテジーパターン予備軍
		switch (m_animStep)
		{
		case animClip_idle:
			m_busyoAnime.Play(animClip_ATK1, INTERPOLATIONTIME_M);
			m_busyoAnimeClip->GetKeyFramePtrListArray();
			//enmuの離れた位置にアタックがあるため、最初だけ+= animClip_ATK1を足す
			m_animStep += animClip_ATK1;
			//マジックナンバーが使われているが、元々単位がある数値に
			//攻撃ごとに敵の吹き飛び微調整をしたかったので
			//1.7fや2.5fなどの倍率をかけています。
			m_blowOffPower = STANDARDPOWER * 1.7f;
			break;
		case animClip_ATK1:
			m_busyoAnime.Play(animClip_ATK2, INTERPOLATIONTIME_M);
			m_animStep++;
			break;
		case animClip_ATK2:
			m_busyoAnime.Play(animClip_ATK3, INTERPOLATIONTIME_M);
			m_animStep++;
			break;
		case animClip_ATK3:
			m_busyoAnime.Play(animClip_ATK4, INTERPOLATIONTIME_M);
			m_animStep++;
			m_blowOffPower = STANDARDPOWER * 2.0f;
			break;
		case animClip_ATK4:
			m_busyoAnime.Play(animClip_ATK5, INTERPOLATIONTIME_M);
			m_animStep++;
			m_blowOffPower = STANDARDPOWER * 2.5f;
			break;
		}
	}
	//無双奥義が打てる状態なら、XAttackMove()を動かす
	if (g_goMgr.isMusouSpecial()) {
		XAttackMove();
	}

	m_playTimer+= GameTime().GetFrameDeltaTime();
	//プレイタイマーが上限値を超えそうなら、上限値のままにする
	if (m_playTimer > PLAYER_TIMER_RIMIT)
	{
		m_playTimer = PLAYER_TIMER_RIMIT;
	}
	if (m_animStep != ANIM_STEP_NO_USE) {
		if (m_animStep != m_oldAnimStep) {
			m_playTimer = TIMER_ZERO;
			m_oldAnimStep = m_animStep;
		}
		if (m_playTimer >= RELEASE_TIME) {
			//一定の時間が過ぎたらアニメステート関係を初期化
			m_busyoState = BusyoNormal;
			m_animStep = animClip_idle;
			m_oldAnimStep = animClip_idle;
			m_playTimer = TIMER_ZERO;
			m_busyoAnime.Play(animClip_idle, INTERPOLATIONTIME_SP);
			m_underAttack = false;
		}
	}
}

void Player::XAttackMove()
{
	//攻撃してないかつYが押されたら
	if (g_pad->IsTrigger(enButtonY)&&!m_underAttack) {
		m_busyoState = BusyoAttack;
		m_busyoAnime.Play(animClip_XATK, INTERPOLATIONTIME_S);
		//マジックナンバーが使われているが、元々単位がある数値に
		//攻撃ごとに敵の吹き飛び微調整をしたかったので
		//3.4fをかけています。
		m_blowOffPower = STANDARDPOWER * 3.4f;
		m_underAttack = true;
		m_XTrigger = true;
	}
	if (m_XTrigger && !m_busyoAnime.IsPlaying()) {
		m_XTrigger = false;
		m_underAttack = false;
		g_goMgr.AddMusouGauge(-MUSOU_SYOUHI);
	}
}

void Player::Execute()
{
	//ワールド座標の更新　こっちのskeletonUpdateをいじる
	auto footStep = m_busyoAnime.Update(GameTime().GetFrameDeltaTime());//ローカル座標の更新　こっちはいじらない
	//if (m_busyoState == BusyoAttack) {
	//攻撃中はフットステップの移動量を加算する。
	CMatrix mBias = CMatrix::Identity();
	mBias.MakeRotationX(CMath::PI * BIAS);
	CMatrix rotMatrix;
	//回転行列を作成する。
	rotMatrix.MakeRotationFromQuaternion(m_rotation);
	rotMatrix.Mul(mBias, rotMatrix);
	rotMatrix.Mul(footStep);

	footStep *= ANIMFOOT_BAIRITU;
	m_moveSpeed += footStep;

	m_position = m_characon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	//ワールド行列の更新。
	m_playerModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);

	
	m_pl_target->HoseiJujge();
}

//エネミーに呼ばれる奴
int Player::RequestEnemyData(CVector3 pos,Enemy* enemy)
{
	for (int i = 0; i < KAKOI_MAX; i++)
	{
		if (!enemy->GetenemyDeath()) {
			//一番最初にエネミーの空いている所に情報を入れる
			if (m_enemydata[i].position.y == Y_ZERO) {
				m_enemydata[i].position = pos;
				m_enemydata[i].enemy = enemy;
				//このタイミングで敵との距離計る
				if (g_goMgr.GetEnemyNum() != ENEMY_NOT_NUM) {
					m_pl_target->SetEnemyInfo(enemy);
				}
				return i;
			}
			else if (m_enemydata[i].enemy == enemy) {
				return i;
			}

			if (m_enemydata[i].position.y != Y_ZERO) {
				CVector3 kyori = m_enemydata[i].position - pos;
				if (kyori.LengthSq() > POSCLEAR_RANGE) {
					m_enemydata[i].enemy = NULL;
					m_enemydata[i].position = CVector3::Zero();
					m_pl_target->Hoseioff();
					return -1;
				}
			}
		}
		else {
			enemy = nullptr;
			m_pl_target->SetEnemyInfo(enemy);
		}
	}
	//nullでないかつ
	//距離が離れていたら
	//-1を戻り値にする
	return -1;
}

//空中攻撃用の動きを処理する関数
void Player::JumpAttackMove() {
	if (m_Jumpfrag && !m_jumpAttackfrag) {
		//
		if (g_pad->IsTrigger(enButtonX)) {
			m_jumpAttackfrag = true;
			m_busyoAnime.Play(animClip_JUMP_ATK, INTERPOLATIONTIME_S);
			m_blowOffPower = JUMPATKPOWER;
		}
		else if(g_pad->IsTrigger(enButtonY)) {
			m_jumpAttackfrag = true;
			m_busyoAnime.Play(animClip_JUMP_X_ATK1, INTERPOLATIONTIME_S);
			m_blowOffPower = JUMPATKPOWER;
			m_busyoState = BusyoAttack_Y;
		}
	}
	if (!m_jumpAttackfrag) {
		m_busyoAnime.Play(animClip_jump, INTERPOLATIONTIME_M);
	}
}

void Player::ThisDamage()
{
	//敵武器のゴーストが当たったらダメージを受ける。
	QueryGOs<Wepon_ghost>("EN_Wepon", [&](Wepon_ghost* wepon) {
		PhysicsGhostObject* ghostobject = wepon->GetGhostObject();
		g_physics.ContactTest(m_characon, [&](const btCollisionObject& contactObject) {
			if (ghostobject->IsSelf(contactObject) == true) {

				PlayerDamage();

				
				g_Effect.m_playEffectHandle = g_Effect.m_effekseerManager->Play(g_Effect.m_sampleEffect, m_position.x, m_position.y + EFFECT_POSITION, m_position.z);
			}
			});
		return true;
		}
	);
}

void Player::DeadPlayer()
{
	m_busyoState = BusyoDead;
	//プレイヤーが死んでいる時の処理
	m_moveSpeed = CVector3::Zero();
	m_busyoAnime.Play(animClip_busyo_dead);
	if (!m_busyoAnime.IsPlaying()) {
		if (m_isDestroyed) {
			Fade::Getinstance().StartFadeIn();
		}
		m_isDestroyed = true;
		if (!Fade::Getinstance().IsFade()) {
			//InGameSoundDirector::GetInstans().UpdateOff();
			g_goMgr.NewGameObject<GameOver>("GameOver");
			InGameSoundDirector::GetInstans().UpdateOff();
			InGameSoundDirector::GetInstans().SoundRelease();
			m_game->GameDelete();
		}
	}
}