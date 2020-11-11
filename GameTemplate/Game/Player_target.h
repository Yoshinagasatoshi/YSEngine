#pragma once
#include "gameObject/ysGameObject.h"
/// <summary>
/// �G�ƃv���C���[�̋����𑪂��Ĉ�ԋ߂�������n��
/// /// </summary>
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
	Player* m_player = nullptr;				//�v���C���[�̃|�C���^
	Enemy* m_enemy = nullptr;				//�G�l�~�[�̃|�C���^
	CVector3 m_distans = CVector3::Zero();	//�v���C���[�ƃG�l�~�[�̋��������鐔�l
	bool m_isHosei = false;					//�␳�������邩�ǂ����̃t���O
	//std::list<Enemy*> m_enemyList;
};

