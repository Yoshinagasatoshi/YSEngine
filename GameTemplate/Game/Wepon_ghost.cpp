/// <summary>
/// このクラス、名前間違えた感がすごい
/// 敵兵orプレイヤーの武器判定をつけているクラス
/// </summary>
#include "stdafx.h"
#include "Wepon_ghost.h"
#include "gameObject/ysGameObjectManager.h"
#include "Player.h"
const float m_ghostscale = 150.0f;

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
	//バグあり。後で直す
	//多分デリートしたghostにアクセスした疑い
	if (!m_hanteifin) {
		GhostInit();
	}
	else{
		DeleteGO(this);
	}
}

void Wepon_ghost::GhostInit()
{
	//nullptrなら
	if (m_player != nullptr) {
		if (!m_hanteifin) {
			m_ghostObject.CreateBox(
				m_player->GetPosition(),
				m_rotation,
				m_scale * m_ghostscale * 1.2f//プレイヤーだけひいきします。いまだけちいさくします
			);
		}
		m_hanteifin = true;
	}
	else {
		if (!m_hanteifin) {
			m_ghostObject.CreateBox(
				m_position,
				m_rotation,
				m_scale * m_ghostscale
			);
		}
		m_hanteifin = true;
	}
}