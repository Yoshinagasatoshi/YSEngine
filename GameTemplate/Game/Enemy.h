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
class Enemy : public IGameObject
{
public:
	Enemy();
	virtual ~Enemy();
	//virtual void Update();
	//virtual void Draw();
	//virtual void Move() = 0;
	CVector3 SetPos(const CVector3 position)
	{
		return m_position = position;
	}
	CQuaternion SetRot(const CQuaternion qrot)
	{
		return m_rotation = qrot;
	}
	void SetPlayerInfo(Player* pl)
	{
		m_player = pl;
	}
protected:
	SkinModel m_model;	//スキンモデル。
	CVector3 m_position; //現在地
	CQuaternion m_rotation; //回転角度
	CVector3 m_scale;
	CVector3 m_playerPos; //プレイヤーのポジション
	Player* m_player; //プレイヤーのポイント型
};

