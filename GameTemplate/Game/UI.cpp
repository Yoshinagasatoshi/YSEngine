#include "stdafx.h"
#include "UI.h"
#include "gameObject/ysGameObjectManager.h"
#include "Player.h"

UI::UI()
{
	m_lifeGauge.Init(L"Assets/sprite/Green.dds", 480.0f, 24.0f);
	//m_sprite.SetPosition(m_position);
	//m_sprite.SetRotation(m_rotation);
	//m_sprite.SetScale(m_scale);
	m_face.Init(L"Assets/sprite/new_Busyo_icon.dds", 80.0f, 80.0f);
	m_mapSprite.Init(L"Assets/sprite/minimap.dds",160.0f, 160.0f);
}

UI::~UI()
{
	g_goMgr.DeleteGOObject(this);
}

void UI::Update()
{
	if (m_isPLInfo) {
		m_playerHP = m_player->GetPlayerHP();
		m_oldPlayerHP = m_player->GetPlayerHP();
		m_lifeGauge.SetPosition(m_position);
		m_isPLInfo = true;
	}

	m_playerHP = m_player->GetPlayerHP();
	if (m_oldPlayerHP != m_playerHP) {
		
		/// <summary>
		/// 画像を小さくしていく。まだ画像が出ないので書けないが、
		/// HP = 横幅の長さに設定して、攻撃を受けたら現在のHPに合わせた
		/// 体力の幅にしていく。
		/// </summary>
		m_lifeGauge.InitCommon(m_playerHP,24.0f);
	}
	m_oldPlayerHP = m_playerHP;
	m_lifeGauge.Update(m_position, m_rotation, m_scale, m_pivot);
	m_face.Update(m_position2,m_rotation2,m_scale2,m_pivot2);
	m_mapSprite.Update(m_position3,m_rotation3,m_scale3,m_pivot3);
}

void UI::PostDraw()
{
	m_lifeGauge.Draw();
	m_face.Draw();
	m_mapSprite.Draw();
}
