#pragma once
#include "Enemy.h"
#include "graphics/Sprite.h"
#include "character/CharacterController.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
class Enemy_Busyo :
	public Enemy
{
public:
	Enemy_Busyo();
	~Enemy_Busyo();

	/// <summary>
	/// 敵武将の場所をセットする
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// 敵武将の場所を受け取る
	/// </summary>
	/// <returns></returns>
	const CVector3& Getposition()const
	{
		return m_position;
	}
	//描写
	void Draw();
	//プレイヤーを発見したときの動き
	void NormalMove();
	//プレイヤーが発見されていない時の動き
	void IdleMove();
	//プレイヤーに攻撃するときの関数
	void AttackMove();
	//いつもの更新処理
	void Update();
	//キャラコンを入れる
	void CharaconInit();
	//enemy_busyoのステートを判定する
	void StateJudge();
	//ダメージ食らった時の処理
	void ThisDamage();
	//ダメージを食らった後の処理
	void DamageAfter();
	//死ぬ処理
	void ThisDelete();
private:
	//攻撃時間が何秒か抽選する関数
	//計測時間も初期化する
	float AttackframeNum()
	{
		m_attackFrameNum = 150.0f + rand() % 200;
		return m_attackFrameNum;
	}
	enum AnimState
	{
		IDL,
		ATK,
		MOVE,
		DAMAGE,
		DAMAGE_AFTER,
		DEATH,
		FIGHTING,
		FIGHTING_KICK,
		FIGHTING_LONG,
		LEFT_STEP,
		AnimationClip_Num
	};
	Animation m_enemy_BusyoAnime;				//敵武将のアニメ管理

	AnimationClip m_animClip[AnimationClip_Num];//アニメーションクリップ
	CVector3 distance;							//距離。
	AnimState m_state;							//アニメーションステート。			
	SkinModel m_model;							//スキンモデル。
	CharacterController m_characon;				//キャラコン
	CVector3 m_moveSpeed = CVector3::Zero();	//移動量を入れる変数。
	CVector3 m_lastPosition;					//直前までいた武将の位置
	float m_mileage = 0;						//移動した量の受け皿。一定値を超えると処理が変わる仕様
	bool m_charaConUse = false;					//キャラコンが入っているか確認する変数。
	bool m_isDeadfrag = false;					//死亡フラグ。
	bool m_isFight = false;						//ファイトポーズをするかどうかのフラグ
	bool m_isDestroyed = false;					//敵武将の死亡フラグ。
	bool m_isATKMode = false;					//攻撃態勢
	bool m_isFightingKick = false;				//飛び蹴りをするかどうかの特殊なフラグ
	float m_attackFrameNum = 0.0f;				//攻撃時間。この時間を超えると攻撃フラグがたつ
	float m_frameTimer = 0.0f;					//時間を図るため
	//自分が出す武器のゴーストのポインタ。
	Wepon_ghost* m_en_Wepon;

	int m_HP = 0;					//仮体力
	int m_gacha = -1;				//攻撃方法の抽選をする変数。最初だけ-1
	ID3D11ShaderResourceView* m_normalMapSRV = nullptr;	//法線マップのSRV
	ID3D11ShaderResourceView* m_specMapSRV = nullptr;	//スペキュラマップのSRV

};

