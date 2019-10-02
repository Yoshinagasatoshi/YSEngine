#pragma once
#include "Enemy.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
/// <summary>
/// Enemy‚Ì”h¶ƒNƒ‰ƒX‘«Œy
/// </summary>
class Enemy_asigaru : public Enemy
{
public:
	Enemy_asigaru();
	~Enemy_asigaru();
	void Update();
	void Draw();
	void Move() override;
	//‘«ŒyƒAƒjƒ
	Animation m_asigaruAnime;
	//F‚ñ‚È‘«ŒyƒAƒjƒ‚ğŠi”[‚µ‚Ä‚¢‚é”z—ñ
	AnimationClip m_asigaruAnimeClip[1];
};

