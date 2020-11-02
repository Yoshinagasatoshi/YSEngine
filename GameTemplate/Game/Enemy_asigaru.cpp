#include "stdafx.h"
#include "Enemy_asigaru.h"
#include "gameObject/ysGameObjectManager.h"
#include "GameData.h"
#include "InGameSoundDirector.h"
#include "Wepon_ghost.h"
#include "GameCamera.h"
#include "math/Matrix.h"

const float Too_close = 7.0f;						//この範囲より近ければ～の条件式に使う。直訳は[近すぎる]
const float Timer_ZERO = 0.0f;						//タイマーをゼロにしたいときに使う
const float DeleteTime = 10.0f;						//倒れる～死ぬまでにかかる時間。
const float drawNearSpeed = 110.0f;					//プレイヤーと距離が近いときのスピード
const float DoubleSpeed = 2.0f;						//倍速したいときに使う。
const float SrowSpeed = 0.1f;						//低速にしたい時に使う。1/10の速さになる
const float earlySpeed = 30.0f;						//超高速したいときに使う。30倍
const float ViewLenght = 0.25f;						//視野角範囲
const float fastTime = 1.0f;						//この数値が大きくなれば音が鳴る時間が長くなる
const int MAX_RING_SE = 10;							//音が出る最大数
const float SoundTimer = 60.0f;						//音ガタセルカを計測するタイマー
const int	Sound_Zero = 0;							//音が鳴るタイマーがないとき
const float FramePlusNum = 1.0f;					//毎フレームごとに足される数。攻撃間隔の調整に使用

const float CULLING_AREA = 8000.0f;					//アップデートや描画する武将モデルの範囲
const float Sound_Interval = 10.0f;					//次の音がなるまでの時間。音が重ならないようにするためのもの
const float Model_Radius = 60.0f;					//敵のモデルの半径。コリジョンを作るために使用
const float Model_Hight = 100.0f;					//敵のモデルの高さ。コリジョンを作るために使用
const float EffectPos_Y = 100.0f;					//エフェクトを出す位置の高さを調整。
const float Musou_GaugePlus = 15.0f;				//無双ゲージがどれだけ足されるのかの数値。調整可能


const float INTERPORATION_TIME_S = 0.1f;//補間時間・小、短めのアニメーション補間をしたいときに使う。 
const float INTERPORATION_TIME_M = 0.2f;//補間時間・中、アニメーション補間をしたいときに使う。

const int HITSTOP_BIG = 8;				//ヒットストップをするフレーム数(大)。無双技を当てた時はこちらを使う
const int HITSTOP_SMALL = 4;			//ヒットストップをするフレーム数(小)。通常技を当てた時はこちらを使う

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
	//m_model_Row.Init(L"Assets/modelData/asigaru_Rowpori.cmo");
	m_rotation = CQuaternion::Identity();
	//アニメーションを格納
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
	//g_goMgr.Counting();
	g_goMgr.EnemyNumSubtract();
	g_goMgr.DeleteGOObject(this);
}

void Enemy_asigaru::CharaconInit()
{
	//キャラコンの初期化
	m_characon.Init(
		Model_Radius,
		Model_Hight,
		m_position//位置
	);
}

void Enemy_asigaru::Update()
{
	if (!m_isFirstUpdate) {
		//最初のアップデートではない。
		//カメラとの距離を計算する。
		CVector3 toCamera = m_position - g_camera3D.GetPosition();
		if (toCamera.LengthSq() > CULLING_AREA * CULLING_AREA) {
			return;
		}
	}
	m_isFirstUpdate = false;
	
	if (isRingSE) {
		timer -= SoundTimer * GameTime().GetFrameDeltaTime();
		//タイマーが0未満になるとリセット。音が出せるようになる
		if (timer < Timer_ZERO) {
			if (m_sd->GetReturnRingNum() <= Sound_Zero) {
				isRingSE = false;
				timer = Sound_Interval;
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
					g_goMgr.Counting();
					g_goMgr.AddMusouGauge(Musou_GaugePlus);

					if (m_player->IsXTrigger()) {
						//XとYで渡す引数の値をを変える。数値が大きければヒットストップの時間が長くなる
						g_goMgr.HitStopOn(HITSTOP_BIG);
					}
					else {
						//Xなので少なめに
						g_goMgr.HitStopOn(HITSTOP_SMALL);
					}
					
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
						m_position.y + EffectPos_Y, //少し上に出す
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
			g_goMgr.EnemyNumSubtract();
			m_model.SetShadowCaster(false);
			//CCount = true;
			DeleteGO(this);
			return;
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
	//モデルの描画
	//消えるときは呼ばない
	if (m_Deathtimer_f < DeleteTime) {
		//最初のアップデートではない。
		//カメラとの距離を計算する。
		CVector3 toCamera = m_position - g_camera3D.GetPosition();
		if (toCamera.LengthSq() > CULLING_AREA * CULLING_AREA) {
			return;
		}

		//CVector3 cameraPos = m_gameCamera->GetCameraPos();
		//CVector3 Lenght = cameraPos - m_position;
		//Lenght.y = 0.0f;	

		m_model.Draw(
			g_camera3D.GetViewMatrix(),
			g_camera3D.GetProjectionMatrix()
		);
	}
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
		if (m_moveSpeed.Length() < Too_close) {
			m_moveSpeed = CVector3::Zero();
		}
		else {
			m_moveSpeed.Normalize();
			m_moveSpeed *= earlySpeed;//30倍
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
	//プレイヤーの位置をキャッチする
	m_playerPos = m_player->GetPosition();

	//最初の五体はプレイヤーを囲むように到着地点を設定
	m_idlePos[0].idlePos = m_playerPos + CVector3(0.0f,0.0f,-150.0f);		//一番奥にいるセンター
	m_idlePos[1].idlePos = m_playerPos + CVector3(100.0f, 0.0f, -100.0f);	//奥にいるやつの隣
	m_idlePos[2].idlePos = m_playerPos + CVector3(150.0f, 0.0f, -30.0f);	//端っこ1
	m_idlePos[3].idlePos = m_playerPos + CVector3(-150.0f, 0.0f, -30.0f);	//端っこ2
	m_idlePos[4].idlePos = m_playerPos + CVector3(-100.0f, 0.0f, -100.0f);	//奥にいるやつの隣2

}

//距離による判定処理
void Enemy_asigaru::StateJudge()
{
	//プレイヤーとの距離を測る
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
		m_frameTimer += FramePlusNum;

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

			//視野角内に入っているか？
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
		m_moveSpeed = kyori * drawNearSpeed * DoubleSpeed; //速度が少し足りなかったので二倍速
		break;


	case Asigaru_totugeki:
		if (kyori.LengthSq() < VigilanceRange) {
			//視野角内に入っているか？
			kyori.Normalize();
			float angle = acosf(kyori.Dot(m_forward));
			if (fabsf(angle) < PI * ViewLenght) {
				m_asigaruState = Asigaru_tikazuki;
			}
		}
		kyori.Normalize();
		m_moveSpeed = kyori * drawNearSpeed * SrowSpeed;
		//今の処理
		//m_moveSpeed = CVector3::Zero();
		CQuaternion ADDrot = CQuaternion::Identity();
		ADDrot.SetRotationDeg(CVector3::AxisY(), INTERPORATION_TIME_S);
		m_rotation.Multiply(ADDrot);
		break;
	}
	m_asigaruAnime.Play(m_asigaruState, INTERPORATION_TIME_M);
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
	m_asigaruAnime.Play(Asigaru_dead, INTERPORATION_TIME_S);
	//死んだアニメーションは早く再生する
	m_asigaruAnime.Update(GameTime().GetFrameDeltaTime());
}
