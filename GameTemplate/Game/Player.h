#pragma once
#include "character/CharacterController.h"
#include "gameObject/ysGameObject.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
//�v���C���[�N���X�A���o�����ɂ�����N���X
class Player : public IGameObject
{
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
	CVector3& GetPosition()
	{
		return m_position;
	}
	//void SetEnemy(Enemy* enemy)
	//{
	//	m_enemy = enemy;
	//}
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
		animClip_num
	};
	AnimationClip m_busyoAnimeClip[animClip_num];
private:
	void AttackMove();									//���o�̍U���̏���������
	int m_animStep = 0;									//�A�j���[�V�������ǂ̒i�K��
	int m_oldAnimStep= 0;								
	int m_playTimer = 0;								//�A�j����������Ăǂꂭ�炢���Ԃ������Ă��邩�B
	SkinModel m_playerModel;							//�X�L�����f���B
	CVector3 m_position = CVector3::Zero();				//�v���C���[�̈ʒu
	CQuaternion m_rotation = CQuaternion::Identity();   //�v���C���[�̉�]
	CVector3 m_scale = CVector3::One();					//�v���C���[�̑傫��
	CVector3 m_moveSpeed = CVector3::Zero();			//�v���C���[�̈ړ����x
	CharacterController m_characon;						//�L�����N�^�[�R���g���[���[
	bool m_Jumpfrag = false;							//�L�����̓W�����v���Ă��邩�H
	//Enemy* m_enemy = nullptr;
};

