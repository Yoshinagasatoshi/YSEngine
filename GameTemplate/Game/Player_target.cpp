#include "stdafx.h"
#include "Player_target.h"
#include "Player.h"
#include "Enemy.h"

void Player_target::IsHosei()
{
	if (m_enemy) {
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
		m_isHosei = false;
	}
}