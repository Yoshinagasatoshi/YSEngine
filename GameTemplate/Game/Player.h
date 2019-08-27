#pragma once
#include "character/CharacterController.h"

class Player
{
public:
	Player();
	~Player();
	void Update();
	void Draw();
private:
	SkinModel m_model;	//スキンモデル。
	CVector3 m_position = CVector3::Zero(); //プレイヤーの位置
	CQuaternion m_rotation = CQuaternion::Identity(); //プレイヤーの回転
	CVector3 m_scale = CVector3::One(); //プレイヤーの大きさ
};

