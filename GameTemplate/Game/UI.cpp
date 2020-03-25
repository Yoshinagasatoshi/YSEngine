#include "stdafx.h"
#include "UI.h"
#include "gameObject/ysGameObjectManager.h"
#include "Player.h"

UI::UI()
{
	m_sprite.Init(L"Assets/sprite/Green.dds", 480.0f, 24.0f);
	//m_sprite.SetPosition(m_position);
	//m_sprite.SetRotation(m_rotation);
	//m_sprite.SetScale(m_scale);
}

UI::~UI()
{

}

void UI::Update()
{
	if (m_isPLInfo) {
		m_playerHP = m_player->GetPlayerHP();
		m_oldPlayerHP = m_player->GetPlayerHP();
		m_sprite.SetPosition(m_position);
		m_isPLInfo = true;
	}

	m_playerHP = m_player->GetPlayerHP();
	if (m_oldPlayerHP != m_playerHP) {

		/// <summary>
		/// 画像を小さくしていく。まだ画像が出ないので書けないが、
		/// HP = 横幅の長さに設定して、攻撃を受けたら現在のHPに合わせた
		/// 体力の幅にしていく。
		/// </summary>
	}
	m_oldPlayerHP = m_playerHP;
	m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);
}

void UI::PostDraw()
{
	m_sprite.Draw();
}
