#pragma once
#include "character/CharacterController.h"
#include "gameObject/ysGameObject.h"
class Player : public IGameObject
{
public:
	Player();
	~Player();
	void Update();
	void Draw();
	void CharaconInit();
	CVector3& GetPosition()
	{
		return m_position;
	}
	//void SetEnemy(Enemy* enemy)
	//{
	//	m_enemy = enemy;
	//}
private:
	int timer = 0;
	SkinModel m_playerModel;							//スキンモデル。
	CVector3 m_position = CVector3::Zero();				//プレイヤーの位置
	CQuaternion m_rotation = CQuaternion::Identity();   //プレイヤーの回転
	CVector3 m_scale = CVector3::One();					//プレイヤーの大きさ
	CVector3 m_moveSpeed = CVector3::Zero();			//プレイヤーの移動速度
	CharacterController m_characon;						//キャラクターコントローラー
	//Enemy* m_enemy = nullptr;
};

