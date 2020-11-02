#include "stdafx.h"
#include "Player_target.h"
#include "Player.h"
#include "Enemy.h"
#include "gameObject/ysGameObjectManager.h"
//�v���C���[���G���a�鎞�ɕ␳�������邩�ǂ����𒲂ׂ�
const float IS_HOSEI_RANGE = 1000.0f;//���̐��l���l���Z����΁A�v���C���[�͕␳��������
void Player_target::HoseiJujge()
{
	if (m_enemy){
		if (!m_enemy->GetenemyDeath()) {
			m_distans = m_player->GetPosition() - m_enemy->GetPos();
			if (m_distans.Length() < IS_HOSEI_RANGE)
			{
				m_isHosei = true;
			}
			else {
				m_isHosei = false;
			}
		}
		else {
			m_enemy = nullptr;
		}
	}
	else {
		m_isHosei = false;
	}
}