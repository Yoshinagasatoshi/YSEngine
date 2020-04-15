#pragma once
#include "graphics/Sprite.h"
#include "gameObject/ysGameObject.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
class GameOver : public IGameObject
{
public:
	GameOver();
	~GameOver();

	void Update();
	void PostDraw();
	void Draw();
	enum animClipNumber {
		anim_Lose,
		animClip_num
	};
private:

	Sprite m_sprite2; //一つ目のスプライト
	//Sprite m_sprite2; //二つ目のスプライト
	SkinModel m_skinModel;//やられたプレイヤーを移す用。
	CVector3 m_position = CVector3::Zero();
	CVector3 m_scale = CVector3::One();
	CQuaternion m_rot = CQuaternion::Identity();
	CVector2 m_pivot = { 0.5f,0.5f };
	AnimationClip m_animClip[animClip_num];
	Animation m_animeModel;
};

