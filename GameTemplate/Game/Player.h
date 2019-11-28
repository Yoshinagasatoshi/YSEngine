#pragma once
#include "character/CharacterController.h"
#include "gameObject/ysGameObject.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
/// <summary>
/// 
/// </summary>
class Enemy;
//プレイヤークラス、無双武将にあたるクラス
class Player : public IGameObject
{
	Enemy* enemy;
public:
	Player();
	~Player();
	//毎回の更新処理
	void Update();
	//毎度の描画処理
	void Draw();
	//恒例の移動処理
	void Move();
	//常設の回転処理
	void Turn();
	//キャラクターのコリジョン初期化
	void CharaconInit();
	//プレイヤーの位置を返す関数
	CVector3& GetPosition()
	{
		return m_position;
	}
	//プレイヤーの回転
	CQuaternion& GetRotation()
	{
		return m_rotation;
	}
	//プレイヤーにダメージが入れたいときに呼ぶ
	void PlayerDamage()
	{
		m_damagefrag = true;
	}
	//足軽アニメ
	Animation m_busyoAnime;
	//色んな武将アニメを格納している配列
	enum busyoBASICAnimeClip {
		animClip_idle = 0,
		animClip_ATK1,
		animClip_ATK2,
		animClip_ATK3,
		animClip_ATK4,
		animClip_ATK5,
		animClip_SmallDamage,
		animClip_num
	};
	AnimationClip m_busyoAnimeClip[animClip_num];
	struct EnemeyData
	{
		CVector3 position = CVector3(NULL, NULL, NULL);
		Enemy* enemy;
	};

	//敵情報のリクエストを受け取る
	int RequestEnemyData(CVector3 position, Enemy* enemy);
private:
	//アニメーションイベントを呼び出すよ
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//囲いの最大数
	static const int kakoi_max = 5;
	EnemeyData m_enemydata[kakoi_max];
	void AttackMove();									//無双の攻撃の処理を書く
	int m_animStep = 0;									//アニメーションがどの段階か
	int m_oldAnimStep= 0;								
	int m_playTimer = 0;								//アニメが流されてどれくらい時間がたっているか。
	SkinModel m_playerModel;							//スキンモデル。
	CVector3 m_position = CVector3::Zero();				//プレイヤーの位置
	CQuaternion m_rotation = CQuaternion::Identity();   //プレイヤーの回転
	CVector3 m_scale = CVector3::One();					//プレイヤーの大きさ
	CVector3 m_moveSpeed = CVector3::Zero();			//プレイヤーの移動速度
	CharacterController m_characon;						//キャラクターコントローラー
	bool m_Jumpfrag = false;							//キャラはジャンプしているか？
	bool m_damagefrag = false;							//ダメージは受けたか？
	int PL_HP = 5;										//今の体力
	float SpeedAmount = 1000.0f;						//平面の移動量
	float gravity = 600.0f;								//重力
	float JumpPower = 1200.0f;							//飛
	//重力が強くかかるようになる奴
	float gravity_keisuu = 0.1f;
};

