#include "stdafx.h"
#include "Komedawara.h"
#include "Player.h"
#include "Onigiri.h"
#include "gameObject/ysGameObjectManager.h"
#include "InGameSoundDirector.h"
const float NEAR_IF = 100.0f*100.0f;	//�v���C���[�Ƌ߂��ƌ�����͈́@�ݏ�

Komedawara::Komedawara()
{
	//�Q�[�����̃��x������Ă΂��\��
	m_komedawaraModel.Init(L"Assets/modelData/komedawara.cmo");
}


Komedawara::~Komedawara()
{
}

void Komedawara::Update()
{
	CVector3 Distans = m_player->GetPosition() - m_position;

	if (Distans.LengthSq() <= NEAR_IF
		&&m_player->IsAttack()) {
		InGameSoundDirector::GetInstans().RingSE_COLLAPSED();
		m_onigiri = g_goMgr.NewGameObject<Onigiri>("Onigiri");
		m_onigiri->SetPosition(m_position);		//�����̈ʒu�ɂ��ɂ�����o��
		m_onigiri->SetPlayer(m_player);			//�v���C���[�̏����󂯓n��
		g_goMgr.DeleteGOObject(this);			//���̌������
	}
	m_komedawaraModel.UpdateWorldMatrix(m_position,m_rot,m_scale);
}

void Komedawara::Draw()
{
	m_komedawaraModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}