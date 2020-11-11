#pragma once
#include "Enemy.h"
#include "graphics/Sprite.h"
#include "character/CharacterController.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
class Enemy_Busyo :
	public Enemy
{
public:
	Enemy_Busyo();
	~Enemy_Busyo();

	/// <summary>
	/// �G�����̏ꏊ���Z�b�g����
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// �G�����̏ꏊ���󂯎��
	/// </summary>
	/// <returns></returns>
	const CVector3& Getposition()const
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
	//�_���[�W��H�������̏���
	void DamageAfter();
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
	enum AnimState
	{
		IDL,
		ATK,
		MOVE,
		DAMAGE,
		DAMAGE_AFTER,
		DEATH,
		FIGHTING,
		FIGHTING_KICK,
		FIGHTING_LONG,
		LEFT_STEP,
		AnimationClip_Num
	};
	Animation m_enemy_BusyoAnime;				//�G�����̃A�j���Ǘ�

	AnimationClip m_animClip[AnimationClip_Num];//�A�j���[�V�����N���b�v
	CVector3 distance;							//�����B
	AnimState m_state;							//�A�j���[�V�����X�e�[�g�B			
	SkinModel m_model;							//�X�L�����f���B
	CharacterController m_characon;				//�L�����R��
	CVector3 m_moveSpeed = CVector3::Zero();	//�ړ��ʂ�����ϐ��B
	CVector3 m_lastPosition;					//���O�܂ł��������̈ʒu
	float m_mileage = 0;						//�ړ������ʂ̎󂯎M�B���l�𒴂���Ə������ς��d�l
	bool m_charaConUse = false;					//�L�����R���������Ă��邩�m�F����ϐ��B
	bool m_isDeadfrag = false;					//���S�t���O�B
	bool m_isFight = false;						//�t�@�C�g�|�[�Y�����邩�ǂ����̃t���O
	bool m_isDestroyed = false;					//�G�����̎��S�t���O�B
	bool m_isATKMode = false;					//�U���Ԑ�
	bool m_isFightingKick = false;				//��яR������邩�ǂ����̓���ȃt���O
	float m_attackFrameNum = 0.0f;				//�U�����ԁB���̎��Ԃ𒴂���ƍU���t���O������
	float m_frameTimer = 0.0f;					//���Ԃ�}�邽��
	//�������o������̃S�[�X�g�̃|�C���^�B
	Wepon_ghost* m_en_Wepon;

	int m_HP = 0;					//���̗�
	int m_gacha = -1;				//�U�����@�̒��I������ϐ��B�ŏ�����-1
	ID3D11ShaderResourceView* m_normalMapSRV = nullptr;	//�@���}�b�v��SRV
	ID3D11ShaderResourceView* m_specMapSRV = nullptr;	//�X�y�L�����}�b�v��SRV

};

