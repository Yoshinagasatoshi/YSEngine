#pragma once
#include "character/CharacterController.h"
#include "gameObject/ysGameObject.h"
class Player : public IGameObject
{
public:
	Player();
	~Player();
	void Update();
	void Draw();
	void CharaconInit();
	CVector3& GetPosition()
	{
		return m_position;
	}
	//void SetEnemy(Enemy* enemy)
	//{
	//	m_enemy = enemy;
	//}
private:
	int timer = 0;
	SkinModel m_playerModel;							//�X�L�����f���B
	CVector3 m_position = CVector3::Zero();				//�v���C���[�̈ʒu
	CQuaternion m_rotation = CQuaternion::Identity();   //�v���C���[�̉�]
	CVector3 m_scale = CVector3::One();					//�v���C���[�̑傫��
	CVector3 m_moveSpeed = CVector3::Zero();			//�v���C���[�̈ړ����x
	CharacterController m_characon;						//�L�����N�^�[�R���g���[���[
	//Enemy* m_enemy = nullptr;
};

