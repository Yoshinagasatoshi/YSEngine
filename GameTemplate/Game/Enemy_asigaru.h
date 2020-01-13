#pragma once
#include "Enemy.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "character/CharacterController.h"
#include "math/Matrix.h"
#include "physics/PhysicsGhostObject.h"
/// <summary>
/// Enemy�̔h���N���X���y
/// </summary>
class GameData;
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
	void Move();
	void Turn();
	//���񂾂Ƃ��̃��[��
	void DeadMove();
	//idlePos�̏�����
	void idlePosInit();
	//�����ɂ�锻�菈���֐�
	void StateJudge();
	//�S�[�X�g��Init
	void ghostInit();

	static Enemy_asigaru& GetInstans()
	{
		static Enemy_asigaru enemy;
		return enemy;
	}

	PhysicsGhostObject* GetGhostObject(){
		return &m_ghostObject;
	}
	//�L�����̏�Ԃ̎��
	enum AsigaruState {
		Asigaru_totugeki = 0,
		Asigaru_tikazuki,
		Asigaru_sentou,
		Asigaru_attack,
		Asigaru_damage,
		Asigaru_dead,
		Asigaru_anim_num
	};
	//���y�A�j��
	Animation m_asigaruAnime;
	//�F��ȑ��y�A�j�����i�[���Ă���z��
	AnimationClip m_asigaruAnimeClip[Asigaru_anim_num];
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
	CVector3 m_moveSpeed = CVector3::Zero();
	CVector3 moveV = CVector3::Zero();
	int i = 0;
private:
	//�A�j���[�V�����C�x���g���Ăяo����
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//�L�����N�^�[�̃R���W����������
	void CharaconInit();
	//ghost!
	PhysicsGhostObject m_ghostObject;
	//���y�̏����X�e�[�g�́H
	AsigaruState m_asigaruState = Asigaru_totugeki;
	//���y�̑O�ɂ���Ă����X�e�[�g,�A�j���𗬂����̔���Ɏg�p
	AsigaruState m_oldState = Asigaru_sentou;
	CharacterController m_characon;
	bool m_characonState = false;
	CVector3 m_forward = CVector3::AxisZ();
	double PI = 3.14159265358979323846f;
	//��邼
	//CMatrix a;
	float kaiten = 0.0f;
	GameData* gamedata = nullptr;

	//���Ԍn
	//�v������
	float m_frameTimer = 0.0f;
	//�U�����ԁB���̎��Ԃ𒴂���ƍU���t���O������
	float m_kougekiframenum = 99.9f;
	//�U�����Ԃ����b�����I����֐�
	//�v�����Ԃ�����������
	float AttackframeNum()
	{
		m_kougekiframenum = 100.0f + rand() % 500;
		return m_kougekiframenum;
	}
	//�����ɏ����Ă���R�[�h
	bool m_isDeadfrag = false;
	//�U�����̃t���O
	bool m_isAttack = false;
	//�v���C���[������ł����Ԃ��ǂ������󂯎��ϐ�
	bool m_player_isdead;
};

