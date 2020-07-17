#pragma once
#include "Enemy.h"
#include "character/CharacterController.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
class Enemy_Busyo :
	public Enemy
{
public:
	Enemy_Busyo();
	~Enemy_Busyo();

	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	CVector3 Getposition()
	{
		return m_position;
	}
	//�`��
	void Draw();
	//�v���C���[�𔭌������Ƃ��̓���
	void NormalMove();
	//�v���C���[����������Ă��Ȃ����̓���
	void IdleMove();
	//�v���C���[�ɍU������Ƃ��̊֐�
	void AttackMove();
	//�����̍X�V����
	void Update();
	//�L�����R��������
	void CharaconInit();
	//enemy_busyo�̃X�e�[�g�𔻒肷��
	void StateJudge();
	//�_���[�W�H��������̏���
	void ThisDamage();
	//���ʏ���
	void ThisDelete();
private:
	//�U�����Ԃ����b�����I����֐�
	//�v�����Ԃ�����������
	float AttackframeNum()
	{
		m_attackFrameNum = 150.0f + rand() % 200;
		return m_attackFrameNum;
	}
	enum State
	{
		IDL = 0,
		ATK,
		MOVE,
		DAMAGE,
		DEATH,
		FIGHTING,
		FIGHTING_KICK,
		FIGHTING_LONG,
		AnimationClip_Num
	};
	Animation m_enemy_BusyoAnime;
	AnimationClip m_animClip[AnimationClip_Num];
	CVector3 distance;			//���̂܂܁I�����ł��B
	State m_state;				//���̂܂�
	SkinModel m_model;			//�X�L�����f���B
	CharacterController m_characon;//�L�����R��
	CVector3 m_moveSpeed = CVector3::Zero();//�ړ��ʂ�����ϐ��B
	bool m_charaConUse = false;//�L�����R���������Ă��邩�m�F����ϐ��B
	bool m_isDeadfrag = false;
	bool m_isFight = false;			//�t�@�C�g�|�[�Y�����邩�ǂ����̃t���O
	bool m_isDestroyed = false;

	float m_attackFrameNum = 0.0f;	//�U�����ԁB���̎��Ԃ𒴂���ƍU���t���O������
	float m_frameTimer = 0.0f;		//���Ԃ�}�邽��
	//�������o������̃S�[�X�g�̃|�C���^�B
	Wepon_ghost* m_en_Wepon;

	int m_HP = 0;					//���̗�
	int m_gacha = -1;				//�U�����@�̒��I������ϐ��B�ŏ�����-1
};

