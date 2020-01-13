#pragma once
#include "character/CharacterController.h"
#include "gameObject/ysGameObject.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "physics/PhysicsGhostObject.h"
/// <summary>
/// 
/// </summary>
class Enemy;
//�v���C���[�N���X�A���o�����ɂ�����N���X
class Player : public IGameObject
{
	Enemy* enemy;
public:
	Player();
	~Player();
	//����̍X�V����
	void Update();
	//���x�̕`�揈��
	void Draw();
	//�P��̈ړ�����
	void Move();
	//��݂̉�]����
	void Turn();
	//�L�����N�^�[�̃R���W����������
	void CharaconInit();
	//�S�[�X�g�̑傫���Ƃ���ݒ�
	void ghostInit();
	//�v���C���[�̈ʒu��Ԃ��֐�
	CVector3& GetPosition()
	{
		return m_position;
	}
	//�v���C���[�̉�]
	CQuaternion& GetRotation()
	{
		return m_rotation;
	}
	//�v���C���[�Ƀ_���[�W�����ꂽ���Ƃ��ɌĂ�
	void PlayerDamage() 
	{
		m_damagefrag = true;
	}
	//�v���C���[�͎���ł����Ԃ��ǂ������擾����
	bool GetPlayerDead()
	{
		return m_deadFrag;
	}
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
	{
		(void)clipName;
		MessageBox(NULL, "attack", "attack", MB_OK);
	}
	PhysicsGhostObject* GetGhostObject() {
		return &m_ghostObject;
	}
	//���y�A�j��
	Animation m_busyoAnime;
	//�F��ȕ����A�j�����i�[���Ă���z��
	enum busyoBASICAnimeClip {
		animClip_idle = 0,
		animClip_ATK1,
		animClip_ATK2,
		animClip_ATK3,
		animClip_ATK4,
		animClip_ATK5,
		animClip_SmallDamage,
		animClip_busyo_dead,
		animClip_num
	};
	busyoBASICAnimeClip m_playerState = animClip_idle;
	AnimationClip m_busyoAnimeClip[animClip_num];
	struct EnemeyData
	{
		CVector3 position = CVector3(NULL, NULL, NULL);
		Enemy* enemy;
	};
	//�v���C���[�ɐ؂�ꂽ�Ƃ�
	void isDead()
	{
		m_attack = true;
	}
	//�G���̃��N�G�X�g���󂯎��
	int RequestEnemyData(CVector3 position, Enemy* enemy);
private:
	//�A�j���[�V�����C�x���g���Ăяo����
	//void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//�͂��̍ő吔
	static const int kakoi_max = 5;
	EnemeyData m_enemydata[kakoi_max];
	void AttackMove();									//���o�̍U���̏���������
	int m_animStep = 0;									//�A�j���[�V�������ǂ̒i�K��
	int m_oldAnimStep= 0;								//�Â��A�j���[�V�����X�e�[�g
	int m_playTimer = 0;								//�A�j����������Ăǂꂭ�炢���Ԃ������Ă��邩�B�P�ʁF�b�B
	SkinModel m_playerModel;							//�X�L�����f���B
	CVector3 m_position = CVector3::Zero();				//�v���C���[�̈ʒu
	CQuaternion m_rotation = CQuaternion::Identity();   //�v���C���[�̉�]
	CVector3 m_scale = CVector3::One();					//�v���C���[�̑傫��
	CVector3 m_moveSpeed = CVector3::Zero();			//�v���C���[�̈ړ����x
	CharacterController m_characon;						//�L�����N�^�[�R���g���[���[
	bool m_Jumpfrag = false;							//�L�����̓W�����v���Ă��邩�H
	bool m_damagefrag = false;							//�_���[�W�͎󂯂����H
	int PL_HP = 5;										//���̗̑�
	bool m_deadFrag = false;							//���S�����Ƃ��̃X�C�b�`
	//�d�͂�����������悤�ɂȂ�z
	float gravity_keisuu = 0.1f;
	bool m_attack = false;
	//ghost!
	PhysicsGhostObject m_ghostObject;
};

