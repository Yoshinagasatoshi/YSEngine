#pragma once
#include "Enemy.h"
#include "character/CharacterController.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
class bom;
class Enemy_Bomber : public Enemy
{
public:
	Enemy_Bomber();
	~Enemy_Bomber();

	void Draw();
	void Update();
	//�X�e�[�g�Ǘ����
	void Statekanri();
	//�A�j���[�V�����Ǘ��W�c
	void Animekanri();

	//�L�����̏�Ԃ̎��
	enum AsigaruState {
		Asigaru_attack,//���̃A�^�b�N�͓�����A�j���[�V����
		Asigaru_zyosou,
		Asigaru_damage,
		Asigaru_dead,
		Asigaru_tikazuki,
		Asigaru_anim_num
	};
	//���e���A�j���B
	Animation m_bomberAnime;
	//�A�j�����i�[���Ă���z��
	AnimationClip m_bomAnimeClip[Asigaru_anim_num];
private:
	//CharacterController m_characon;//�L�����R��
	AsigaruState m_state = Asigaru_zyosou;
	bom* m_bom;
	CVector3 m_moveSpeed;
};