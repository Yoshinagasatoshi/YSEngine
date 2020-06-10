#pragma once
#include "Enemy.h"
#include "character/CharacterController.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
class Enemy_Bomber : public Enemy
{
public:
	Enemy_Bomber();
	~Enemy_Bomber();

	void Draw();
	void Update();
	//�L�����̏�Ԃ̎��
	enum AsigaruState {
		Asigaru_attack,//���̃A�^�b�N�͓�����A�j���[�V����
		Asigaru_zyosou,
		Asigaru_damage,
		Asigaru_dead,
		Asigaru_anim_num
	};
	//���e���A�j���B
	Animation m_bomberAnime;
	//�A�j�����i�[���Ă���z��
	AnimationClip m_bomAnimeClip[Asigaru_anim_num];
private:
	//CharacterController m_characon;//�L�����R��
};