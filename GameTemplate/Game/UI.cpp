#include "stdafx.h"
#include "UI.h"
#include "gameObject/ysGameObjectManager.h"
#include "Player.h"
#include "Game.h"

const float GAUGE_HEIGHT = 24.0f;		//�Q�[�W�̏c�̕�
const float GAUGEURA_HEIGHT = 25.0f;	//�Q�[�W���̗���
const float GAUGE_WEIGHT = 480.0f;		//�Q�[�W�̉���
const float GAUGEURA_WEIGHT = 484.0f;	//���o�Q�[�W���̉���
const float MUSOUGAUGE_HEIGHT = 454.0f;	//���o�Q�[�W���̗���
const float MUSOU_GAUGEMAX = 450.0f;	//���o�Q�[�W�̍ő�l
const float FACEAICON_WEIGHT = 80.0f;	//�����̊�A�C�R���̉���
const float FACEAICON_HEIGHT = 80.0f;	//�����̊�A�C�R���̏c��
const float MINIMAP_WEIGHT = 160.0f;	//�~�j�}�b�v�̉���
const float MINIMAP_HEIGHT = 160.0f;	//�~�j�}�b�v�̗���
const float MINI_PL_AICON_WEIGHT = 8.0f;//�~�j�}�b�v��̃v���C���[�A�C�R���̉���
const float MINI_PL_AICON_HEIGHT = 8.0f;//�~�j�}�b�v��̃v���C���[�A�C�R���̉���
const float TIMELIMIT = 6000.0f;		//�������Ԃ̐ݒ�

const int TEXT_MAX = 255;				//�e�L�X�g��������l

const float minimap_tekiou = 62.0f;		//�~�j�}�b�v�ɓK�����邽�߂̐��l
const float RESET = 0.0f;				//0�ɂ������Ƃ��Ɏg��
const float mainasu_Vector = -1.0f;		//�x�N�g�����t�����ɂ������Ƃ��Ɏg��
const CVector4 Color = CVector4{ 1.0f,0.0f,0.0f,1.0f };//�t�H���g�̐F��ς���BR,G,B,A�̏��Ԃɕ���ł܂��B

const int ColorChangeNum = 10;			//�F�ւ����邽�߂ɕK�v�Ȑ�

const CVector2 TOUBATU_SU_FONTPOS = { 0.0f,650.0f }; //�������̃t�H���g�̈ʒu
const CVector2 TIMER_FONT_POS = { 0.0f,0.0f };		 //�������Ԃ̃t�H���g�̈ʒu
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
		/// HP = �����̒����ɐݒ肵�āA�U�����󂯂��猻�݂�HP�ɍ��킹��
		/// �̗͂̕��ɂ��Ă����B
		/// </summary>
		m_lifeGauge.InitCommon(m_playerHP, GAUGE_HEIGHT);
	}
	m_oldPlayerHP = m_playerHP;

	if (g_goMgr.GetCount() > ColorChangeNum)
	{
		m_defeatColor = CVector4{ 0.85f,0.70f,0.0f,1.0f };
	}

	//���o�Q�[�W�@����
	//�Q�[�W��MAX�ł͂Ȃ��� => ���Ԍo�߁A�G���a�邱�ƂŃQ�[�W�����܂�B
	//�Q�[�W��Max�Ȏ� => �������؂��Ă����܂�Ȃ��BX�{�^�����g�������ɃQ�[�W������
	timer++;
	//���n�@��Œ����܂��B
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
	PlayerPos.z = RESET;	//�O�������͂���Ȃ��̂�0�ɂ���

	CQuaternion PlayerRot = m_player->GetRotation();
	//�v���C���[�|�C���^
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

	swprintf_s(text, L"���j�� : %03d", defeadNum);
	
	
	m_font.DrawScreenPos(text, TOUBATU_SU_FONTPOS,m_defeatColor);


	wchar_t teext[TEXT_MAX];
	float RemainingTime = TIMELIMIT - timer;
	swprintf_s(teext, L"�������� : %03f",  RemainingTime);
	m_timeFont.DrawScreenPos(teext,TIMER_FONT_POS,Color);
	
	//m_playerPointer_yazirushi.Draw();
}
