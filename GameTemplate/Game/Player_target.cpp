#include "stdafx.h"
#include "Player_target.h"
#include "Player.h"
#include "Enemy.h"
#include "gameObject/ysGameObjectManager.h"
//プレイヤーが敵を斬る時に補正をかけるかどうかを調べる
const float IS_HOSEI_RANGE = 1000.0f;//この数値より値が短ければ、プレイヤーは補正をかける
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