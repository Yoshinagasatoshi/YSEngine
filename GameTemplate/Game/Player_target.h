#pragma once
#include "gameObject/ysGameObject.h"
/// <summary>
/// �˔��I�ɍ���Ă��܂����N���X
/// �G�ƃv���C���[�̋����𑪂��Ĉ�ԋ߂�������n��
/// �����v���C���[�N���X
///	�ςȃN���X���ق�ƂɁc
/// </summary>
class Enemy;
class Player;
class Player_target : public IGameObject
{
public:
	//�v���C���[�̃f�[�^������
	void SetPlayerInfo(Player* pl)
	{
		m_player = pl;
	}
	//�G�̃f�[�^������
	void SetEnemyInfo(Enemy* en)
	{
		m_enemy = en;
	}
	//������Ԃ�
	CVector3 GetDistans()
	{
		return m_distans;
	}
	//�␳�X�C�b�`���m�F
	bool GetHosei()
	{
		return m_isHosei;
	}
	//�␳�X�C�b�`���I�t��
	void Hoseioff()
	{
		m_isHosei = false;
	}
	void HoseiJujge();
private:
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	CVector3 m_distans = CVector3::Zero();
	bool m_isHosei = false;
	//std::list<Enemy*> m_enemyList;
	int i = 0;//�e�X�g�R�[�h
};

