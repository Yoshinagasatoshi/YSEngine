#include "stdafx.h"
#include "Onigiri.h"
#include "gameObject/ysGameObjectManager.h"
#include "Player.h"
#include "InGameSoundDirector.h"
const CVector3 AXIS_Y = { 0.0f,1.0f,0.0f };	//Y��
const float ADD_ROT = 5.0f;					//���t���[���ǂꂾ����]�����邩
const float GET_ITEM_RANGE = 100.0f;			//�A�C�e�����Q�b�g�����Ƃ�����͈�
Onigiri::Onigiri()
{
	m_onigiriModel.Init(L"Assets/modelData/onigiri.cmo");
}


Onigiri::~Onigiri()
{
}

void Onigiri::Update()
{

	//��]�p�x�𖈉񑫂�
	m_rotValue += ADD_ROT;
	//�o�Ă����炭�邭����
	m_rot.SetRotationDeg(AXIS_Y,m_rotValue);
	//�G�t�F�N�g�Ō��炷�@�������񕜂��ď�����
	m_onigiriModel.UpdateWorldMatrix(m_position, m_rot, m_scale);
	CVector3 Distance = m_position - m_player->GetPosition();

	//�v���C���[�Ƌ߂���Ύ擾�����ɂȂ�
	if (Distance.Length() < GET_ITEM_RANGE) {
		GetAitem();
		InGameSoundDirector::GetInstans().RingSE_HEAL();
		m_player->PLAYER_HEAL();
	}

}

void Onigiri::Draw()
{
	//���ɂ��胂�f���̕`��
	m_onigiriModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void Onigiri::GetAitem()
{
	//�v���C���[�ɉ񕜂�^���ď�����
	g_goMgr.DeleteGOObject(this);
}