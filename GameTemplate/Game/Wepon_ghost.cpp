/// <summary>
/// 敵兵orプレイヤーの攻撃判定をつけているクラス
/// </summary>
#include "stdafx.h"
#include "Wepon_ghost.h"
#include "gameObject/ysGameObjectManager.h"
#include "Player.h"
const float GHOST_SCALE = 200.0f;
const float GHOST_TIMER_RIMIT = 2.0f;
const float GHOST_TIMER_RESET = 0.0f;
const float GHOST_TIME_RADD = 30.0f;
Wepon_ghost::Wepon_ghost()
{
	
}

Wepon_ghost::~Wepon_ghost()
{
	
}

//bool Wepon_ghost::Start()
//{
//	return true;
//}

void Wepon_ghost::Update()
{
	m_ghostTimer += GHOST_TIME_RADD * GameTime().GetFrameDeltaTime();
	//表示されている時間が2.0ｆ以上なら消える
	if (m_ghostTimer > GHOST_TIMER_RIMIT)
	{
		m_ghostTimer = GHOST_TIMER_RESET;
		DeleteGO(this);
	}
}

void Wepon_ghost::GhostInit()
{
	//nullptrなら
	//if (m_player != nullptr) {
	if(m_playerCall){
		if (!m_hanteifin) {
			m_ghostObject.CreateBox(
				m_player->GetPosition(),
				m_rotation,
				m_scale * GHOST_SCALE// *1.5f//1.2f//プレイヤーだけ当たり判定を大きく
			);
		}
		m_hanteifin = true;
	}
	else {
		if (!m_hanteifin) {
			m_ghostObject.CreateBox(
				m_position,
				m_rotation,
				m_scale * GHOST_SCALE
			);

			m_player->PlayerDamage();
		}
		m_hanteifin = true;

	}
}