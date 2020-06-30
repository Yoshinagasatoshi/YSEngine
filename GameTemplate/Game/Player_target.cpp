#include "stdafx.h"
#include "Player_target.h"
#include "Player.h"
#include "Enemy.h"
#include "gameObject/ysGameObjectManager.h"
//–¼‘O•åW’†
void Player_target::HoseiJujge()
{
	if (m_enemy){
		if (!m_enemy->GetenemyDeath()) {
			m_distans = m_player->GetPosition() - m_enemy->GetPos();
			if (m_distans.Length() < 1000.0f)
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