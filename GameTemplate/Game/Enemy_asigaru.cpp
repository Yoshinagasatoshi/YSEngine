#include "stdafx.h"
#include "Enemy_asigaru.h"
#include "gameObject/ysGameObjectManager.h"
#include "GameData.h"
#include "InGameSoundDirector.h"
#include "Wepon_ghost.h"
#include "GameCamera.h"
#include "math/Matrix.h"

const float tikadukisugi = 2.0f;
const float Timer_ZERO = 0.0f;
const float DeleteTime = 10.0f;
const float drawNearSpeed = 110.0f;
const float ViewLenght = 0.25f;						//視野角範囲
const float fastTime = 1.0f;						//この数値が大きくなれば音が鳴る時間が長くなる
const int MAX_RING_SE = 10;

/// <summary>
/// boid
/// </summary>
Enemy_asigaru::IdlePos Enemy_asigaru::m_idlePos[5];
//コンストラクタ
Enemy_asigaru::Enemy_asigaru()
{
	gamedata = &GameData::GetInstans();
	m_scale = CVector3::One();
	m_sd = &InGameSoundDirector::GetInstans();
	//m_sd = g_goMgr.NewGameObject<SoundDirector>("SoundDirector");

	//asigaruのモデルをロードする。
	m_model.Init(L"Assets/modelData/asigaru.cmo");
	m_model_Row.Init(L"Assets/modelData/asigaru_Rowpori.cmo");
	m_rotation = CQuaternion::Identity();
	//アニメーションを格納だ
	m_asigaruAnimeClip[Asigaru_totugeki].Load(L"Assets/animData/asigaru_totugeki.tka");
	m_asigaruAnimeClip[Asigaru_tikazuki].Load(L"Assets/animData/asigaru_tikazuki.tka");
	m_asigaruAnimeClip[Asigaru_sentou].Load(L"Assets/animData/asigaru_tikazuki.tka");
	m_asigaruAnimeClip[Asigaru_attack].Load(L"Assets/animData/asigaru_Normal_Attack.tka");
	m_asigaruAnimeClip[Asigaru_damage].Load(L"Assets/animData/asigaru_damage.tka");
	m_asigaruAnimeClip[Asigaru_dead].Load(L"Assets/animData/asigaru_dead.tka");

	m_asigaruAnimeClip[Asigaru_totugeki].SetLoopFlag(true);
	m_asigaruAnimeClip[Asigaru_tikazuki].SetLoopFlag(true);
	m_asigaruAnimeClip[Asigaru_sentou].SetLoopFlag(false);
	m_asigaruAnimeClip[Asigaru_damage].SetLoopFlag(false);
	m_asigaruAnimeClip[Asigaru_dead].SetLoopFlag(false);

	//モデルとアニメデータを初期化
	m_asigaruAnime.Init(
		m_model,
		m_asigaruAnimeClip,
		Asigaru_anim_num
	);
	//下のコードはskeletonの名前を検索している。
	m_skeleton = &m_model.GetSkeleton();
	const wchar_t* bonename[30];

	for (int i = 0; i < 20; i++) {
		bonename[i] = m_skeleton->GetBone(i)->GetName();

		if (i == 19)
		{
			bonename[i + 1] = L"end";
		}
	}

	//ghostInit();


	m_asigaruAnime.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName) {
		(void)clipName;
		m_en_Wepon = g_goMgr.NewGameObject<Wepon_ghost>("EN_Wepon");
		m_en_Wepon->SetPosition(m_position);
		m_en_Wepon->SetPlayerInfo(m_player);
		m_en_Wepon->GhostInit();
		}
	);
	//g_goMgr.InitEffekseer();
	g_Effect.m_sampleEffect = Effekseer::Effect::Create(g_Effect.m_effekseerManager, (const EFK_CHAR*)L"Assets/effect/sword.efk");
	//エフェクトを再生する。
	g_Effect.m_playEffectHandle = g_Effect.m_effekseerManager->Play(g_Effect.m_sampleEffect, 0.0f, 0.0f, 0.0f);
}
//デストラクタ
Enemy_asigaru::~Enemy_asigaru()
{
	//敵を倒した数を計測
	g_goMgr.Counting();
	g_goMgr.DeleteGOObject(this);
	g_goMgr.EnemyNumSubtract();
}

void Enemy_asigaru::CharaconInit()
{
	//キャラコンの初期化
	m_characon.Init(
		60.0f, //半径
		100.0f,//高さ
		m_position//位置
	);
}

void Enemy_asigaru::Update()
{
	if (!m_isFirstUpdate) {
		//最初のアップデートではない。
		//カメラとの距離を計算する。
		CVector3 toCamera = m_position - g_camera3D.GetPosition();
		if (toCamera.LengthSq() > 4000.0f * 4000.0f) {
			return;
		}
	}
	m_isFirstUpdate = false;
	
	if (isRingSE) {
		timer -= 60.0f * GameTime().GetFrameDeltaTime();
		//タイマーが0未満になるとリセット。音が出せるようになる
		if (timer < 0.0f) {
			if (m_sd->GetReturnRingNum() <= 0) {
				isRingSE = false;
				timer = 10.0f;
			}
			////なっている効果音が0じゃなかったら減らす
			//else if (!m_sd->GetReturnRingNum() == 0) {
			//	m_sd->mainasuRingnum();
			//}

		}
	}
	//Y成分の移動速度をバックアップしておく。
	float ySpeed = m_moveSpeed.y;

	//キャラコンが作られていなかったらinitする
	if (!m_characonConUse) {
		m_characonConUse = true;
		CharaconInit();
	}

	if (!m_isDeadfrag) {
		PlayertoDistans();
		//武器のゴーストが自分たちに当たったら、死んだという信号を立てる
		//攻撃判定内ならもう一回呼ばれる可能性があるから、最初に呼ばれたら絶対これを呼ばない
		QueryGOs<Wepon_ghost>("PL_Wepon", [&](Wepon_ghost* wepon) {
			PhysicsGhostObject* ghostobject = wepon->GetGhostObject();
			g_physics.ContactTest(m_characon, [&](const btCollisionObject& contactObject) {
				if (ghostobject->IsSelf(contactObject) == true) {
					//if (m_player->IsXTrigger()) {
					//	//XとYで渡す引数を変える
					//	g_goMgr.HitStopOn(8);
					//}
					//else {
					//	//Xなので少なめに
					//	g_goMgr.HitStopOn(4);
					//}
					//通っているのは確認完了
					m_isDeadfrag = true;
					//enemy用にも
					ThisDeath();
					//斬撃音を鳴らす関数
					m_sd->RingSE_Slash();
					//エフェクトも出す。
					g_Effect.m_playEffectHandle = g_Effect.m_effekseerManager->Play(
						g_Effect.m_sampleEffect,
						m_position.x,
						m_position.y + 100.0f, //少し上に出す
						m_position.z
					);
				}
				});
			return true;
			});
	}

	//打ちあがって着地したときに動いてしまうので
//無理やり動きを固定する
	if (m_isDeadfrag
		&&m_characon.IsOnGround()) {
		m_moveSpeed = CVector3::Zero();

		//音をロード。1回だけ鳴らす。
		if (m_Deathtimer_f <= fastTime) {
			//downの音
			m_sd->RingSE_Down();
		}
		m_Deathtimer_f++;

		//死んだ後に少したってモデルが消える
		if (m_Deathtimer_f > DeleteTime) {
			//m_characon.RemoveRigidBoby();
			g_goMgr.EnemyNumSubtract();
			//g_goMgr.DeleteGOObject(this);
		}
	}

	//死んでいるかどうかで行う処理が変わる
	if (m_isDeadfrag
		&& !m_deadMoveStopper) {
		m_deadMoveStopper = true;
		DeadMove();
		ySpeed = m_moveSpeed.y;
	}
	else if (!m_isDeadfrag) {
		//回転処理。プレイヤーの方に向くだけ
		Turn();
		StateJudge();
	}

	//ワールド座標の更新
	m_moveSpeed.y = ySpeed + grabity;

	
	//m_position += m_moveSpeed;
	m_position = m_characon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	
	//Effekseerカメラ行列を設定。
	//まずはEffeseerの行列型の変数に、カメラ行列とプロジェクション行列をコピー。
	m_asigaruAnime.Update(GameTime().GetFrameDeltaTime());
}
void Enemy_asigaru::Draw()
{
	//最初のアップデートではない。
	//カメラとの距離を計算する。
	CVector3 toCamera = m_position - g_camera3D.GetPosition();
	if (toCamera.LengthSq() > 8000.0f * 8000.0f) {
		return;
	}

	CVector3 cameraPos = m_gameCamera->GetCameraPos();
	CVector3 Lenght = cameraPos - m_position;
	Lenght.y = 0.0f;
	//モデルの描画
	m_model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

//ここが　取り巻く処理を書いている場所
void Enemy_asigaru::Move()
{
	const int NomalMove = -1;
	int kakoi_num = m_player->RequestEnemyData(m_position, this);
	//周辺を取り巻くやつらなのか？
	if (kakoi_num != NomalMove) {
		m_torimaki = true;
		//ここから下は取り巻くやつの処理
		CVector3 kaiten = m_playerPos - m_position;
		//ここだけ動きと回転の処理を変える
		idlePosInit();
		m_moveSpeed = m_idlePos[kakoi_num].idlePos - m_position;
		//目的地と距離があまりにも近いときは動かない
		if (m_moveSpeed.Length() < 7.0f) {
			m_moveSpeed = CVector3::Zero();
		}
		else {
			m_moveSpeed.Normalize();
			m_moveSpeed *= 30.0f;//30倍
		}
	float angle = atan2(kaiten.x,kaiten.z);
	m_rotation.SetRotation(CVector3::AxisY(), angle);
	return;
	}
	else {
		//取り巻きフラグオフ
		m_torimaki = false;
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

	CVector3 aho = m_playerPos - m_position;
	if (aho.Length() < 300.0f) {
		aho = aho;
	}
	else {
		aho.y = 0.0f;
		aho.Normalize();
		aho *= 300.0f;
	}
	//問題は6対名港
	m_idlePos[0].idlePos = m_playerPos + CVector3(0.0f,0.0f,-150.0f);		//一番奥にいるセンター
	m_idlePos[1].idlePos = m_playerPos + CVector3(100.0f, 0.0f, -100.0f);	//奥にいるやつの隣
	m_idlePos[2].idlePos = m_playerPos + CVector3(150.0f, 0.0f, -30.0f);	//端っこ1
	m_idlePos[3].idlePos = m_playerPos + CVector3(-150.0f, 0.0f, -30.0f);	//端っこ2
	m_idlePos[4].idlePos = m_playerPos + CVector3(-100.0f, 0.0f, -100.0f);	//奥にいるやつの隣2

}

//距離による判定処理
void Enemy_asigaru::StateJudge()
{
	//プレイヤーが死んだときにも図られてしまう
	//～～Range 
	/// <summary>
	/// -282497216
	/// -87308032
	/// </summary>
	CVector3 kyori = m_player->GetPosition() - m_position;
	//y成分を0にする。
	kyori.y = 0.0f;

	m_player_isdead = m_player->GetPlayerDead();
	//ステートごとの処理を書く
	switch (m_asigaruState)
	{
	case Asigaru_attack:
		Move();
		m_moveSpeed = CVector3::Zero();
		if (!m_asigaruAnime.IsPlaying()) {
			m_asigaruState = Asigaru_sentou;
		}
		break;
	case Asigaru_sentou:
		if (m_isAttack) {
			m_isAttack = false;
		}	
		if (kyori.LengthSq() > BattleRange) {
			m_asigaruState = Asigaru_tikazuki;
			m_frameTimer = Timer_ZERO;
			AttackframeNum();
		}
		//sentouの処理
		m_moveSpeed = CVector3::Zero();
		m_frameTimer += 1.0f;

		if (m_frameTimer >= m_kougekiframenum)
		{
			m_frameTimer = Timer_ZERO;
			AttackframeNum();

			m_asigaruState = Asigaru_attack;
		}
		Move();
		break;
	case Asigaru_tikazuki:
		if (kyori.LengthSq() < BattleRange) {
			kyori.Normalize();
			float angle = acosf(kyori.Dot(m_forward));

			//視野角入ってる？
			if (fabsf(angle) < PI * ViewLenght) {
				m_asigaruState = Asigaru_sentou;
			}
		}
		else if (kyori.LengthSq() > VigilanceRange)
		{
			m_asigaruState = Asigaru_totugeki;
		}
		//tikazukiの処理
		kyori.Normalize();
		m_moveSpeed = kyori * drawNearSpeed * 2.0f;
		break;


	case Asigaru_totugeki:
		if (kyori.LengthSq() < VigilanceRange) {
			//視野角入ってる？
			kyori.Normalize();
			float angle = acosf(kyori.Dot(m_forward));
			if (fabsf(angle) < PI * ViewLenght) {
				m_asigaruState = Asigaru_tikazuki;
			}
		}
		kyori.Normalize();
		m_moveSpeed = kyori * drawNearSpeed * 0.1f;
		//今の処理
		//m_moveSpeed = CVector3::Zero();
		CQuaternion ADDrot = CQuaternion::Identity();
		ADDrot.SetRotationDeg(CVector3::AxisY(), 0.1f);
		m_rotation.Multiply(ADDrot);
		break;
	}
	m_asigaruAnime.Play(m_asigaruState, 0.2f);
}

/// <summary>
/// 死んだときに呼ばれる名前の通りの関数
/// </summary>
void Enemy_asigaru::DeadMove()
{
	//当たり判定などの開放
	//m_ghostObject.Release();
	//プレイヤーから食らった技の威力に応じて
	//吹き飛ぶ量を決める
	float power;
	power = m_player->GetBlowOffPower();
	m_moveSpeed.y += power;
	CVector3 pushPower;
	pushPower = m_forward * power;
	m_moveSpeed -= pushPower;
	m_asigaruAnime.Play(Asigaru_dead,0.1f);
	//死んだアニメーションは早く再生する
	m_asigaruAnime.Update(GameTime().GetFrameDeltaTime());
}
