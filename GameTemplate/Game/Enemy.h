#pragma once
#include "gameObject/ysGameObject.h"
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
	~Enemy();
	void Update();
	void Draw();
	void Move();
	//後で削除予定。レベルが機能しているかどうかの確認コード
	CVector3 SetPos(const CVector3 position)
	{
		return m_position = position;
	}
	int timer = 0;
private:
	SkinModel m_model;	//スキンモデル。
	CVector3 m_position = CVector3{ 100.0f,-100.0f,0.0f }; //敵の位置、でもレベルで配置するので仮
	CQuaternion m_rotation = CQuaternion{ 0.0f,0.0f,0.0f,1.0f };//こいつのレベル
	CVector3 m_scale = CVector3::One(); //敵の大きさこいつもレベルだ
	Player* m_player;
};

