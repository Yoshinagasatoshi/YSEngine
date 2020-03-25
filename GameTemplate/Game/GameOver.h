#pragma once
#include "graphics/Sprite.h"
#include "gameObject/ysGameObject.h"
class GameOver : public IGameObject
{
public:
	GameOver();
	~GameOver();
private:
	void Update();
	void PostDraw();
	Sprite m_sprite1; //一つ目のスプライト
	Sprite m_sprite2; //二つ目のスプライト
	CVector3 m_position = CVector3::Zero();
	CVector3 m_scale = CVector3::One();
	CQuaternion m_rot = CQuaternion::Identity();
	CVector2 m_pivot = {0.5f,0.5f};
};

