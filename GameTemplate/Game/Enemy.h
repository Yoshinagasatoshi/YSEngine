#pragma once
#include "gameObject/ysGameObject.h"
#include "Player.h"
/// <summary>
/// 敵兵クラス。
/// <remarks>
/// 敵の基本的な動きを与えるクラスです
/// 足軽や敵将クラスの基底クラスになる予定
/// </remarks>
/// </summary>
class Player;
class GameCamera;
class Game;
class Enemy : public IGameObject
{
public:
	Enemy();
	virtual ~Enemy();
	//virtual void Update();
	//virtual void Draw();
	//virtual void Move() = 0;
	const CVector3& SetPos(const CVector3& position)
	{
		return m_position = position;
	}
	const CVector3& GetPos()const
	{
		return m_position;
	}

	const CQuaternion& SetRot(const CQuaternion& qrot)
	{
		m_rotation = qrot;
		return m_rotation;
	}
	void SetPlayerInfo(Player* pl)
	{
		m_player = pl;
	}
	void SetGameCameraInfo(GameCamera* camera)
	{
		m_gameCamera = camera;
	}
	void SetGameinfo(Game* game)
	{
		m_game = game;
	}
	//プレイヤーとエネミーの距離をはかる
	//m_playerに情報が入っている時だけつかう
	void PlayertoDistans()
	{
		m_PtDistans = m_player->GetPosition() - m_position;
	}
	const CVector3& GetPlayertoDistans()const
	{
		return m_PtDistans;
	}

	//プレイヤーに倒されたら呼ばれる関数
	void ThisDeath()
	{
		m_isDeath = true;
	}

	bool GetenemyDeath() const
	{
		return m_isDeath;
	}
protected:
	SkinModel m_model;			//スキンモデル。
	SkinModel m_model_Row;		//ローポリのスキンモデル。アニメはつけない予定
	CVector3 m_position;		//現在地
	CQuaternion m_rotation;		//回転角度
	CVector3 m_scale;			//大きさ
	CVector3 m_playerPos;		//プレイヤーのポジション
	Player* m_player;			//プレイヤーのポイント型
	GameCamera* m_gameCamera;	//ゲームカメラのポイント型。
	Game* m_game;				//ゲーム
	CVector3 m_PtDistans;		//プレイヤーとエネミーの距離
	const float grabity = -9.8f * 2.0f;//重力
	const float BattleRange = 200.0f * 200.0f;			//この距離の範囲内に近づくとバトル(2乗)
	const float VigilanceRange = 700.0f * 700.0f;		//この距離の範囲内ならプレイヤーに近づく(2乗)

	bool m_isDeath = false;		//エネミーの死亡フラグ
};

