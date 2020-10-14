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
	//�X�e�[�g�Ǘ�
	void Statekanri();
	//�A�j���[�V�����Ǘ�
	void Animekanri();
	//��]
	void Turn();
	//����
	void EscapeMove();
	//�U���B
	void attackMove();
	//���ʁB
	void DeathMove();
	//Move�̃f�B���N�^�[�B�ǂ̃��[�u���J��o�������߂�B
	void Move();
	//�L�����R�[�����쐬
	void CharaconInit();
	//�L�����̏�Ԃ̎��
	enum AsigaruState {
		Asigaru_attack,//���̃A�^�b�N�͓�����A�j���[�V����
		Asigaru_zyosou,
		Asigaru_damage,
		Asigaru_dead,
		Asigaru_escape,
		Asigaru_Idle,
		Asigaru_anim_num
	};
	//���e���A�j���B
	Animation m_bomberAnime;
	//�A�j�����i�[���Ă���z��
	AnimationClip m_bomAnimeClip[Asigaru_anim_num];
private:
	CharacterController m_characon;//�L�����R��
	AsigaruState m_state = Asigaru_zyosou;//�ŏ��̏�Ԃ͏������Ă��郂�[�h
	bom* m_bom;//�{�����o�����ߕt��
	CVector3 m_moveSpeed = CVector3::Zero();//�ړ����x
	bool m_isCharaconUse = false;//�L�����R�������Ă�H
	bool m_isDeadfrag = false;	//bomber�����񂾂��ǂ���
};