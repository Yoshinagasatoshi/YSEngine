#pragma once
#include "gameObject/ysGameObject.h"
#include "graphics/Sprite.h"
class GameClear : public IGameObject
{
public:
	GameClear();
	~GameClear();
private:
	void Update();	
	void PostDraw();
	Sprite m_sprite1; //��ڂ̃X�v���C�g
	Sprite m_sprite2; //��ڂ̃X�v���C�g
	CVector3 m_position = CVector3::Zero();
	CVector3 m_scale = CVector3::One();
	CQuaternion m_rot = CQuaternion::Identity();
	CVector2 m_pivot = { 0.5f,0.5f };
};

