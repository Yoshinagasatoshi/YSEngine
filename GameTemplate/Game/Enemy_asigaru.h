#pragma once
#include "Enemy.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
/// <summary>
/// Enemy�̔h���N���X���y
/// </summary>
class Enemy_asigaru : public Enemy
{
public:
	Enemy_asigaru();
	~Enemy_asigaru();
	void Update();
	void Draw();
	void Move() override;
	//���y�A�j��
	Animation m_asigaruAnime;
	//�F��ȑ��y�A�j�����i�[���Ă���z��
	AnimationClip m_asigaruAnimeClip[1];
};

