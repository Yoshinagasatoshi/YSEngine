#pragma once
#include "character/CharacterController.h"
#include "gameObject/ysGameObject.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "physics/PhysicsGhostObject.h"
#include "graphics/Skeleton.h"
#include "sound/SoundSource.h"

/// <summary>
/// 
/// </summary>
class Game;
class Enemy;
//class Skeleton;
class Wepon_ghost;
class Player_target;
class InGameSoundDirector;
//�v���C���[�N���X�A���o�����ɂ�����N���X
class Player : public IGameObject
{
	int testID = -1;
	Enemy* enemy;
public:
	Player();
	~Player();
	//�X�V����
	void Update();
	//�`�揈��
	void Draw();
	//�ړ�����
	void Move();
	//��]����
	void Turn();
	//�o�͏���
	void Execute();
	//�L�����N�^�[�̃R���W����������
	void CharaconInit();
	//�v���C���[�̈ʒu��Ԃ��֐�
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	//�v���C���[�̉�]
	const CQuaternion& GetRotation() const
	{
		return m_rotation;
	}
	//�v���C���[�Ƀ_���[�W�����ꂽ���Ƃ��ɌĂ�
	void PlayerDamage() 
	{
		m_damagefrag = true;
	}
	//�v���C���[�͎���ł����Ԃ��ǂ������擾����
	const bool& GetPlayerDead()const
	{
		return m_deadFrag;
	}
	//�v���C���[�̐�����΂��͂��擾
	const float& GetBlowOffPower()const
	{
		return m_blowOffPower;
	}
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
	{
		(void)clipName;
		MessageBox(NULL, "attack", "attack", MB_OK);
	}
	const CVector3& GetCalcPos() const
	{
		return m_calcPos;
	}
	//�v���C���[��HP���󂯎��
	const int& GetPlayerHP() const
	{
		return m_PL_HP;
	}

	//�v���C���[�����G�ɂȂ�
	void SetPlayerMuTeki()
	{
		m_muteki = true;
	}

	//�����̏�Ԃ��i�[
	enum busyoState {
		BusyoNormal = 0,//�ʏ�̕���
		BusyoAttack,	//�U����Ԃ̕���
		BusyoAttack_Y,	//�W�����v����Y���������Ƃ��̃X�e�[�g
		BusyoDamage,	//�U����H����Ă��鎞�̕���
		BusyoDead		//���S��������
	};
	busyoState m_busyoState = BusyoNormal;

	//���y�A�j��
	Animation m_busyoAnime;
	//�F��ȕ����A�j�����i�[���Ă���z��BbusyoState�ɔ��f�����B
	enum busyoAnimeClip {
		animClip_idle = 0,
		animClip_Walk,
		animClip_jump,
		animClip_ATK1,
		animClip_ATK2,
		animClip_ATK3,
		animClip_ATK4,
		animClip_ATK5,
		animClip_XATK,
		animClip_JUMP_ATK,
		animClip_JUMP_X_ATK1,
		animClip_JUMP_X_ATK2,
		animClip_SmallDamage,
		animClip_busyo_dead,
		animClip_num
	};
	busyoAnimeClip m_animState = animClip_idle;
	AnimationClip m_busyoAnimeClip[animClip_num];
	void SetGameInfo(Game* game)
	{
		m_game = game;
	}
	
	struct EnemeyData
	{
		CVector3 position = CVector3(0.0f,0.0f,0.0f);
		Enemy* enemy;
	};
	//�v���C���[�ɐ؂�ꂽ�Ƃ�
	void isDead()
	{
		m_dead = true;
	}
	//�G���̃��N�G�X�g���󂯎��
	int RequestEnemyData(CVector3 position, Enemy* enemy);
	void SetEnemyData(Enemy*en)
	{
		enemy = en;
	}
	//X�{�^����������Ă��邩�̔���
	const bool IsXTrigger()const
	{
		return m_XTrigger;
	}
private:
	//�͂��̐�
	static const int DestinationNum = 5;
	void AttackMove();									//���o�̍U���̏���������
	void XAttackMove();									//X�{�^���������ꂽ�Ƃ��̏���
	void JumpAttackMove();								//�W�����v�A�^�b�N�̏����B
	void ThisDamage();									//�_���[�W���󂯂��Ƃ��̏���
	int m_animStep = 0;									//�A�j���[�V�������ǂ̒i�K��
	int m_oldAnimStep= 0;								//�Â��A�j���[�V�����X�e�[�g
	float m_playTimer = 0.0f;							//�A�j����������Ăǂꂭ�炢���Ԃ������Ă��邩�B�P�ʁF�b�B
	int m_TimerRelease = 13;							//�X�e�[�g����������܂ł̗P�\���ԁB20�͒P��
	int m_PL_HP = 480;									//���̗̑� 24 �f�o�b�N�p�̐����ł�
	float m_gravity_keisuu = 0.1f;						//�d�͂�����������悤�ɂȂ�W���B1.0f�����
	float m_blowOffPower;								//�G�𐁂���΂��З�
	float WideMoveL;									//L�X�e�B�b�N��X���͗ʂ��󂯎��
	float heightMoveL;									//L�X�e�B�b�N��Y���͗ʂ��󂯎��B

	EnemeyData m_enemydata[DestinationNum];				//�G�l�~�[���s�����W�̃f�[�^�������Ă���B
	SkinModel m_playerModel;							//�X�L�����f���B
	CVector3 m_position = CVector3::Zero();				//�v���C���[�̈ʒu
	CQuaternion m_rotation = CQuaternion::Identity();   //�v���C���[�̉�]
	CVector3 m_scale = CVector3::One();					//�v���C���[�̑傫��
	CVector3 m_moveSpeed = CVector3::Zero();			//�v���C���[�̈ړ����x
	CVector3 m_CameraForward = g_camera3D.GetForword();	//�J�����̑O�������擾
	CVector3 m_CameraRight = g_camera3D.GetRight();		//�J�����̉E�������擾
	CVector3 m_calcPos;									//�{�[���Ǝ����̈ʒu�̍��v
	Skeleton* m_skelton;								//Player�̃X�P���g���f�[�^
	CharacterController m_characon;						//�L�����N�^�[�R���g���[���[
	Game* m_game;										//�Q�[���̃C���X�^���X���i�[����

	bool m_dead = false;								//���S�X�C�b�`�B���������Ԃ��Ă����Ȃ������̂ł��ƂŒ���
	bool m_deadFrag = false;							//���S�����Ƃ��̃X�C�b�`
	bool m_underAttack = false;							//�U�����H
	bool m_Jumpfrag = false;							//�L�����̓W�����v���Ă��邩�H
	bool m_damagefrag = false;							//�v���C���[�Ƀ_���[�W��^�������_���[�W�H
	bool m_jumpAttackfrag = false;						//�W�����v�A�^�b�N���H
	bool m_gamefinal = false;							//�I��肾�c
	bool m_isDestroyed = false;							//1�x�̂݃Q�[���I�[�o�[���Ă�
	bool m_plWeponSwhich = true;
	bool m_XTrigger = false;
	bool m_muteki = false;								//���G�X�C�b�`�A�_���[�W��H���Ȃ��Ȃ�B
	
	CSoundSource m_bgm;
	CSoundSource m_se;									//���ʉ�
	CSoundSource m_se2;									//���ʉ�


	PhysicsGhostObject m_ghostObject;					//�v���C���[�{�̂̃S�[�X�g
	Wepon_ghost* m_pl_Wepon = nullptr;					//����̃S�[�X�g���o���Bpl�̓v���C���[���o���S�[�X�g
	Player_target* m_pl_target = nullptr;				//�v���C���[�^�[�Q�b�g
	InGameSoundDirector* m_sd;						//�炷
	ID3D11ShaderResourceView* m_normalMapSRV = nullptr;


};

