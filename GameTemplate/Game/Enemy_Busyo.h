#pragma once
#include "Enemy.h"
#include "character/CharacterController.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
class Enemy_Busyo :
	public Enemy
{
public:
	Enemy_Busyo();
	~Enemy_Busyo();

	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	CVector3 Getposition()
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
	enum State
	{
		IDL = 0,
		ATK,
		MOVE,
		DAMAGE,
		DEATH,
		FIGHTING,
		FIGHTING_KICK,
		FIGHTING_LONG,
		AnimationClip_Num
	};
	Animation m_enemy_BusyoAnime;
	AnimationClip m_animClip[AnimationClip_Num];
	CVector3 distance;			//そのまま！距離です。
	State m_state;				//そのまま
	SkinModel m_model;			//スキンモデル。
	CharacterController m_characon;//キャラコン
	CVector3 m_moveSpeed = CVector3::Zero();//移動量を入れる変数。
	bool m_charaConUse = false;//キャラコンが入っているか確認する変数。
	bool m_isDeadfrag = false;
	bool m_isFight = false;			//ファイトポーズをするかどうかのフラグ
	bool m_isDestroyed = false;

	float m_attackFrameNum = 0.0f;	//攻撃時間。この時間を超えると攻撃フラグがたつ
	float m_frameTimer = 0.0f;		//時間を図るため
	//自分が出す武器のゴーストのポインタ。
	Wepon_ghost* m_en_Wepon;

	int m_HP = 0;					//仮体力
	int m_gacha = -1;				//攻撃方法の抽選をする変数。最初だけ-1
};

