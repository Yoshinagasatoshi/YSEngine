#pragma once
#include "gameObject/ysGameObject.h"
/// <summary>
/// 敵兵クラス。
/// <remarks>
/// 敵の基本的な動きを与えるクラスです
/// 足軽や敵将クラスの基底クラスになる予定
/// </remarks>
/// </summary>
class Enemy : public IGameObject
{
public:
	Enemy();
	~Enemy();
	void Update();
	void Draw();
private:
	SkinModel m_model;	//スキンモデル。
	CVector3 m_position = CVector3{ 100.0f,-100.0f,0.0f }; //敵の位置、でもレベルで配置するので仮
	CQuaternion m_rotation = CQuaternion{ 0.0f,0.0f,0.0f,1.0f };//こいつのレベル
	CVector3 m_scale = CVector3::One(); //敵の大きさこいつもレベルだ
};

