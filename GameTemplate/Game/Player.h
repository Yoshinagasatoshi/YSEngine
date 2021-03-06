#pragma once
#include "character/CharacterController.h"
#include "gameObject/ysGameObject.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "physics/PhysicsGhostObject.h"
#include "graphics/Skeleton.h"
#include "sound/SoundSource.h"

/// <summary>
/// プレイヤークラス
/// </summary>
class Game;
class Enemy;
//class Skeleton;
class Wepon_ghost;
class Player_target;
class InGameSoundDirector;
//プレイヤークラス、無双武将にあたるクラス
class Player : public IGameObject
{
	int testID = -1;
	Enemy* enemy;
public:
	Player();
	~Player();
	//更新処理
	void Update();
	//描画処理
	void Draw();
	//移動処理
	void Move();
	//回転処理
	void Turn();
	//出力処理
	void Execute();
	//死亡処理
	void DeadPlayer();
	//キャラクターのコリジョン初期化
	void CharaconInit();
	//プレイヤーの位置を返す関数
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	//プレイヤーの回転
	const CQuaternion& GetRotation() const
	{
		return m_rotation;
	}
	//プレイヤーにダメージが入れたいときに呼ぶ
	void PlayerDamage() 
	{
		m_damagefrag = true;
	}
	/// <summary>
	/// プレイヤーは死んでいる状態かを決める
	/// </summary>
	/// <returns></returns>
	const bool& GetPlayerDead()const
	{
		return m_deadFrag;
	}
	/// <summary>
	/// プレイヤーの吹き飛ばし力を設定
	/// </summary>
	/// <returns></returns>
	const float& GetBlowOffPower()const
	{
		return m_blowOffPower;
	}
	/// <summary>
	/// アニメーションイベントが行われているかのチェックコード
	/// </summary>
	/// <param name="clipName"></param>
	/// <param name="eventName"></param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
	{
		(void)clipName;
		MessageBox(NULL, "attack", "attack", MB_OK);
	}
	const CVector3& GetCalcPos() const
	{
		return m_calcPos;
	}
	//プレイヤーのHPを受け取る
	const int& GetPlayerHP() const
	{
		return m_PL_HP;
	}

	//プレイヤーが無敵になる
	void SetPlayerMuTeki()
	{
		m_muteki = true;
	}
	//プレイヤーがどんな状況でも絶命する
	void ConpulsionDead()
	{
		m_PL_HP = 0;
		m_deadFrag = true;
	}
	//プレイヤーが回復する
	void PLAYER_HEAL()
	{
		m_PL_HP += m_PL_HP_MAX / 4;//25%回復する
		//体力最大値を超えそうなら、体力最大値のままに
		if (m_PL_HP >= m_PL_HP_MAX) {
			m_PL_HP = m_PL_HP_MAX;
		}
	}
	//武将の状態を格納
	enum busyoState {
		BusyoNormal = 0,//通常の武将
		BusyoAttack,	//攻撃状態の武将
		BusyoAttack_Y,	//ジャンプ中にYを押したときのステート
		BusyoDamage,	//攻撃を食らっている時の武将
		BusyoDead		//死亡した武将
	};
	busyoState m_busyoState = BusyoNormal;

	//足軽アニメ
	Animation m_busyoAnime;
	//色んな武将アニメを格納している配列。busyoStateに反映される。
	enum busyoAnimeClip {
		animClip_idle = 0,
		animClip_Walk,
		animClip_jump,
		animClip_ATK1,
		animClip_ATK2,
		animClip_ATK3,
		animClip_ATK4,
		animClip_ATK5,
		animClip_XATK,
		animClip_JUMP_ATK,
		animClip_JUMP_X_ATK1,
		animClip_JUMP_X_ATK2,
		animClip_SmallDamage,
		animClip_busyo_dead,
		animClip_num
	};
	busyoAnimeClip m_animState = animClip_idle;
	AnimationClip m_busyoAnimeClip[animClip_num];
	void SetGameInfo(Game* game)
	{
		m_game = game;
	}
	
	struct EnemeyData
	{
		CVector3 position = CVector3(0.0f,0.0f,0.0f);
		Enemy* enemy;
	};
	//プレイヤーに切られたとき
	void isDead()
	{
		m_dead = true;
	}
	//敵情報のリクエストを受け取る
	int RequestEnemyData(CVector3 position, Enemy* enemy);
	void SetEnemyData(Enemy*en)
	{
		enemy = en;
	}
	//Xボタンが押されているかの判定
	const bool IsXTrigger()const
	{
		return m_XTrigger;
	}
	//攻撃したかどうかのフラグ
	const bool IsAttack()const
	{
		return m_underAttack;
	}
private:
	//囲いの数
	static const int DestinationNum = 5;
	void AttackMove();									//無双の攻撃の処理を書く
	void XAttackMove();									//Xボタンが押されたときの処理
	void JumpAttackMove();								//ジャンプアタックの処理。
	void ThisDamage();									//ダメージを受けたときの処理
	int m_animStep = 0;									//アニメーションがどの段階か
	int m_oldAnimStep= 0;								//古いアニメーションステート
	float m_playTimer = 0.0f;							//アニメが流されてどれくらい時間がたっているか。単位：秒。
	int m_TimerRelease = 13;							//ステートが解放されるまでの猶予時間。20は単位
	int m_PL_HP = 480;									//今の体力
	int m_PL_HP_MAX = 480;								//体力の最大値
	float m_gravity_keisuu = 0.1f;						//重力が強くかかるようになる係数。1.0fが上限
	float m_blowOffPower;								//敵を吹き飛ばす威力
	float WideMoveL;									//LスティックのX入力量を受け取る
	float heightMoveL;									//LスティックのY入力量を受け取る。

	EnemeyData m_enemydata[DestinationNum];				//エネミーが行く座標のデータが入っている。
	SkinModel m_playerModel;							//スキンモデル。
	CVector3 m_position = CVector3::Zero();				//プレイヤーの位置
	CQuaternion m_rotation = CQuaternion::Identity();   //プレイヤーの回転
	CVector3 m_scale = CVector3::One();					//プレイヤーの大きさ
	CVector3 m_moveSpeed = CVector3::Zero();			//プレイヤーの移動速度
	CVector3 m_CameraForward = g_camera3D.GetForword();	//カメラの前方向を取得
	CVector3 m_CameraRight = g_camera3D.GetRight();		//カメラの右方向を取得
	CVector3 m_calcPos;									//ボーンと自分の位置の合計
	Skeleton* m_skelton;								//Playerのスケルトンデータ
	CharacterController m_characon;						//キャラクターコントローラー
	Game* m_game;										//ゲームのインスタンスを格納する

	bool m_dead = false;								//死亡スイッチ。役割がかぶってそうなやつがいるのであとで直す
	bool m_deadFrag = false;							//死亡したときのスイッチ
	bool m_underAttack = false;							//攻撃中？
	bool m_Jumpfrag = false;							//キャラはジャンプしているか？
	bool m_damagefrag = false;							//プレイヤーにダメージを与えたかダメージ？
	bool m_jumpAttackfrag = false;						//ジャンプアタック中？
	bool m_gamefinal = false;							//ゲームが終わるフラグ
	bool m_isDestroyed = false;							//1度のみゲームオーバーを呼ぶ
	bool m_XTrigger = false;							//必殺技対応のボタンが押されているかのフラグ
	bool m_muteki = false;								//無敵スイッチ、ダメージを食らわなくなる。
	//
	//CSoundSource m_bgm;									//ＢＧＭ
	//CSoundSource m_se;									//効果音
	//CSoundSource m_se2;									//効果音

	PhysicsGhostObject m_ghostObject;					//プレイヤー本体のゴースト
	Wepon_ghost* m_pl_Wepon = nullptr;					//武器のゴーストを出す。plはプレイヤーが出すゴースト
	Player_target* m_pl_target = nullptr;				//プレイヤーターゲット
	InGameSoundDirector* m_sd;							//鳴らす
	ID3D11ShaderResourceView* m_normalMapSRV = nullptr;	//法線マップのSRV


};

