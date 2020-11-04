#include "stdafx.h"
#include "UI.h"
#include "gameObject/ysGameObjectManager.h"
#include "Player.h"
#include "Game.h"

const float GAUGE_HEIGHT = 24.0f;		//ゲージの縦の幅
const float GAUGEURA_HEIGHT = 25.0f;	//ゲージ裏の立幅
const float GAUGE_WEIGHT = 480.0f;		//ゲージの横幅
const float GAUGEURA_WEIGHT = 484.0f;	//無双ゲージ裏の横幅
const float MUSOUGAUGE_HEIGHT = 454.0f;	//無双ゲージ裏の立幅
const float MUSOU_GAUGEMAX = 450.0f;	//無双ゲージの最大値
const float FACEAICON_WEIGHT = 80.0f;	//武将の顔アイコンの横幅
const float FACEAICON_HEIGHT = 80.0f;	//武将の顔アイコンの縦幅
const float MINIMAP_WEIGHT = 160.0f;	//ミニマップの横幅
const float MINIMAP_HEIGHT = 160.0f;	//ミニマップの立幅
const float MINI_PL_AICON_WEIGHT = 8.0f;//ミニマップ状のプレイヤーアイコンの横幅
const float MINI_PL_AICON_HEIGHT = 8.0f;//ミニマップ状のプレイヤーアイコンの横幅
const float TIMELIMIT = 6000.0f;		//制限時間の設定

const int TEXT_MAX = 255;				//テキスト文字上限値

const float minimap_tekiou = 62.0f;		//ミニマップに適応するための数値
const float RESET = 0.0f;				//0にしたいときに使う
const float mainasu_Vector = -1.0f;		//ベクトルを逆方向にしたいときに使う
const CVector4 Color = CVector4{ 1.0f,0.0f,0.0f,1.0f };//フォントの色を変える。R,G,B,Aの順番に並んでます。

const int ColorChangeNum = 10;			//色替えするために必要な数

const CVector2 TOUBATU_SU_FONTPOS = { 0.0f,650.0f }; //討伐数のフォントの位置
const CVector2 TIMER_FONT_POS = { 0.0f,0.0f };		 //制限時間のフォントの位置
UI::UI()
{
	isTimeOver = false;
	m_lifeGauge.Init(L"Assets/sprite/Green.dds", GAUGE_WEIGHT, GAUGE_HEIGHT);
	m_lifeGaugeura.Init(L"Assets/sprite/Green_ura.dds", GAUGEURA_WEIGHT, GAUGEURA_HEIGHT);

	m_musouGauge.Init(L"Assets/sprite/musougauge.dds", MUSOU_GAUGEMAX, GAUGE_HEIGHT);
	m_musouGaugeura.Init(L"Assets/sprite/Green_ura.dds", MUSOUGAUGE_HEIGHT, GAUGEURA_HEIGHT);
	

	//m_sprite.SetPosition(m_position);
	//m_sprite.SetRotation(m_rotation);
	//m_sprite.SetScale(m_scale);
	m_face.Init(L"Assets/sprite/new_Busyo_icon.dds", FACEAICON_WEIGHT, FACEAICON_HEIGHT);
	m_mapSprite.Init(L"Assets/sprite/minimap.dds", MINIMAP_WEIGHT, MINIMAP_HEIGHT);
	m_playerPointer.Init(L"Assets/sprite/PlayerPointer.dds", MINI_PL_AICON_WEIGHT, MINI_PL_AICON_HEIGHT);
	//m_playerPointer_yazirushi.Init(L"Assets/sprite/PlayerPointer_yazirusi.dds", 16.0f, 16.0f);

}

UI::~UI()
{
	g_goMgr.DeleteGOObject(this);
}

void UI::Update()
{
	if (!isTimeOver) {
		if (timer >= TIMELIMIT) {
			m_player->ConpulsionDead();
			isTimeOver = true;
		}
	}
	const CVector3& lifeVarPos = CVector3::Zero();
	if (m_isPLInfo) {
		m_playerHP = m_player->GetPlayerHP();
		m_oldPlayerHP = m_player->GetPlayerHP();
		m_lifeGauge.SetPosition(m_position);
		m_lifeGaugeura.SetPosition(lifeVarPos);

		m_isPLInfo = true;
	}

	m_playerHP = m_player->GetPlayerHP();
	if (m_oldPlayerHP != m_playerHP) {
		
		/// <summary>
		/// HP = 横幅の長さに設定して、攻撃を受けたら現在のHPに合わせた
		/// 体力の幅にしていく。
		/// </summary>
		m_lifeGauge.InitCommon(m_playerHP, GAUGE_HEIGHT);
	}
	m_oldPlayerHP = m_playerHP;

	if (g_goMgr.GetCount() > ColorChangeNum)
	{
		m_defeatColor = CVector4{ 0.85f,0.70f,0.0f,1.0f };
	}

	//無双ゲージ　草案
	//ゲージがMAXではない時 => 時間経過、敵を斬ることでゲージがたまる。
	//ゲージがMaxな時 => もちろん切ってもたまらない。Xボタンを使った時にゲージが減る
	timer++;
	//直地　後で直します。
	hyouzi = g_goMgr.GetMusouGaugeValue();
	if (hyouzi <= MUSOU_GAUGEMAX) {
		g_goMgr.SetMusou_Utenai();
	}
	else {
		g_goMgr.SetMusou_Uteru();
	}
	//float timeVal = 0.75;
	//hyouzi += timeVal;
	m_musouGauge.InitCommon(hyouzi, GAUGE_HEIGHT);

	CVector3 PlayerPos = m_player->GetPosition();
	PlayerPos /= minimap_tekiou;
	PlayerPos.x *= mainasu_Vector;
	PlayerPos.y = PlayerPos.z * mainasu_Vector;
	PlayerPos.z = RESET;	//三次元情報はいらないので0にする

	CQuaternion PlayerRot = m_player->GetRotation();
	//プレイヤーポインタ
	m_playerPointer.Update(m_position4 + PlayerPos, m_rotation,m_scale,m_pivot2);
	//m_playerPointer_yazirushi.Update(m_position4 + PlayerPos, m_rotation, m_scale, m_pivot2);

	m_lifeGauge.Update(m_position, m_rotation, m_scale, m_pivot);
	m_lifeGaugeura.Update(m_position, m_rotation, m_scale, m_pivot);

	m_musouGauge.Update(m_musouPos, m_rotation, m_scale, m_pivot);
	m_musouGaugeura.Update(m_musouPos, m_rotation, m_scale, m_pivot);

	m_face.Update(m_position2,m_rotation2,m_scale2,m_pivot2);
	m_mapSprite.Update(m_position3,m_rotation3,m_scale3,m_pivot3);

	
}

void UI::PostDraw()
{
	m_lifeGaugeura.Draw();
	m_lifeGauge.Draw();

	m_musouGaugeura.Draw();
	m_musouGauge.Draw();	

	m_face.Draw();
	m_mapSprite.Draw();
	m_playerPointer.Draw();
	wchar_t text[TEXT_MAX];
	int defeadNum = g_goMgr.GetCount();

	swprintf_s(text, L"撃破数 : %03d", defeadNum);
	
	
	m_font.DrawScreenPos(text, TOUBATU_SU_FONTPOS,m_defeatColor);


	wchar_t teext[TEXT_MAX];
	float RemainingTime = TIMELIMIT - timer;
	swprintf_s(teext, L"制限時間 : %03f",  RemainingTime);
	m_timeFont.DrawScreenPos(teext,TIMER_FONT_POS,Color);
	
	//m_playerPointer_yazirushi.Draw();
}
