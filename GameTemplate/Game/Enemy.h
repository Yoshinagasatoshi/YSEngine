#pragma once
#include "gameObject/ysGameObject.h"
#include "Player.h"
/// <summary>
/// �G���N���X�B
/// <remarks>
/// �G�̊�{�I�ȓ�����^����N���X�ł�
/// ���y��G���N���X�̊��N���X�ɂȂ�\��
/// </remarks>
/// </summary>
class Player;
class Enemy : public IGameObject
{
public:
	Enemy();
	virtual ~Enemy();
	//virtual void Update();
	//virtual void Draw();
	virtual void Move() = 0;
	//��ō폜�\��B���x�����@�\���Ă��邩�ǂ����̊m�F�R�[�h
	CVector3 SetPos(const CVector3 position)
	{
		return m_position = position;
	}
	void SetPlayerInfo(Player* pl)
	{
		m_player = pl;
	}
protected:
	SkinModel m_model;	//�X�L�����f���B
	CVector3 m_position; //���ݒn
	CQuaternion m_rotation; //��]�p�x
	CVector3 m_scale; //�G�̑傫��
	CVector3 m_playerPos; //�v���C���[�̃|�W�V����
	Player* m_player; //�v���C���[�̃|�C���g�^
};

