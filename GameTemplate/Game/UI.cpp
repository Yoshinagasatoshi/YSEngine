#include "stdafx.h"
#include "UI.h"
#include "gameObject/ysGameObjectManager.h"
#include "Player.h"

UI::UI()
{
	m_lifeGauge.Init(L"Assets/sprite/Green.dds", 480.0f, 24.0f);
	m_lifeGaugeura.Init(L"Assets/sprite/Green_ura.dds", 484.0f, 25.0f);

	//m_sprite.SetPosition(m_position);
	//m_sprite.SetRotation(m_rotation);
	//m_sprite.SetScale(m_scale);
	m_face.Init(L"Assets/sprite/new_Busyo_icon.dds", 80.0f, 80.0f);
	m_mapSprite.Init(L"Assets/sprite/minimap.dds",160.0f, 160.0f);
	m_playerPointer.Init(L"Assets/sprite/PlayerPointer.dds", 8.0f, 8.0f);
	//m_playerPointer_yazirushi.Init(L"Assets/sprite/PlayerPointer_yazirusi.dds", 16.0f, 16.0f);

}

UI::~UI()
{
	g_goMgr.DeleteGOObject(this);
}

void UI::Update()
{
	CVector3 a = CVector3::Zero();
	if (m_isPLInfo) {
		m_playerHP = m_player->GetPlayerHP();
		m_oldPlayerHP = m_player->GetPlayerHP();
		m_lifeGauge.SetPosition(m_position);
		m_lifeGaugeura.SetPosition(a);

		m_isPLInfo = true;
	}

	m_playerHP = m_player->GetPlayerHP();
	if (m_oldPlayerHP != m_playerHP) {
		
		/// <summary>
		/// HP = 横幅の長さに設定して、攻撃を受けたら現在のHPに合わせた
		/// 体力の幅にしていく。
		/// </summary>
		m_lifeGauge.InitCommon(m_playerHP,24.0f);
	}
	m_oldPlayerHP = m_playerHP;

	CVector3 PlayerPos = m_player->GetPosition();
	PlayerPos /= 62.0f;
	PlayerPos.x *= -1.0f;
	PlayerPos.y = PlayerPos.z * -1.0f;
	PlayerPos.z = 0.0f;

	CQuaternion PlayerRot = m_player->GetRotation();
	//プレイヤーポインタ
	m_playerPointer.Update(m_position4 + PlayerPos, m_rotation,m_scale,m_pivot2);
	//m_playerPointer_yazirushi.Update(m_position4 + PlayerPos, m_rotation, m_scale, m_pivot2);

	m_lifeGauge.Update(m_position, m_rotation, m_scale, m_pivot);
	m_lifeGaugeura.Update(m_position, m_rotation, m_scale, m_pivot);

	m_face.Update(m_position2,m_rotation2,m_scale2,m_pivot2);
	m_mapSprite.Update(m_position3,m_rotation3,m_scale3,m_pivot3);
}

void UI::PostDraw()
{
	m_lifeGaugeura.Draw();
	m_lifeGauge.Draw();
	m_face.Draw();
	m_mapSprite.Draw();
	m_playerPointer.Draw();
	//m_playerPointer_yazirushi.Draw();
}
