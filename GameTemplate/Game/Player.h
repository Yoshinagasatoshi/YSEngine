#pragma once
#include "character/CharacterController.h"

class Player
{
public:
	Player();
	~Player();
	void Update();
	void Draw();
private:
	SkinModel m_model;	//�X�L�����f���B
	CVector3 m_position = CVector3::Zero(); //�v���C���[�̈ʒu
	CQuaternion m_rotation = CQuaternion::Identity(); //�v���C���[�̉�]
	CVector3 m_scale = CVector3::One(); //�v���C���[�̑傫��
};

