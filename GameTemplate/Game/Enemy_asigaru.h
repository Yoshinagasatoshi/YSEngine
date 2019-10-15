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
	//�A�v�f
	void Update();
	//�`��
	void Draw();
	//����
	void Move() override;
	//idlePos�̏�����
	void idlePosInit();
	//�����ɂ�锻�菈���֐�
	void LenghtJudge();
	//���y�A�j��
	Animation m_asigaruAnime;
	//�F��ȑ��y�A�j�����i�[���Ă���z��
	AnimationClip m_asigaruAnimeClip[1];
	struct IdlePos
	{
		//����̊i�[�ꏊ
		CVector3 idlePos;
		//�i�[�ꏊ���g���Ă��邩
		bool m_isUse = false;
		//���������s��ꂽ���H
		bool m_Syokika = false;
	};
	//�z�񐔂͑ҋ@�ꏊ�̐��B���܂͂S
	static IdlePos m_idlePos[5];
	CVector3 moveSpeed;
	CVector3 moveV = CVector3::Zero();
	int i = 0;
private:
	enum AsigaruState {
		Asigaru_idle = 0, 
		Asigaru_totugeki,
		Asigaru_sentou
	};
	AsigaruState m_state;
};

