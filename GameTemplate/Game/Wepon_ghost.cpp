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
	g_goMgr.DeleteGOObject(this);
}

//bool Wepon_ghost::Start()
//{
//	return true;
//}

void Wepon_ghost::Update()
{
	//�o�O����B��Œ���
	//�����f���[�g����ghost�ɃA�N�Z�X�����^��
	GhostInit();
}

void Wepon_ghost::GhostInit()
{
	if (m_hanteifin) {
		DeleteGO(this);
	}
	//nullptr�Ȃ�
	if (m_player != nullptr) {
		if (!m_hanteifin) {
			m_ghostObject.CreateBox(
				m_player->GetPosition(),
				m_rotation,
				m_scale * m_ghostscale
			);
			m_hanteifin = true;
		}
	}
	else {
		if (!m_hanteifin) {
			m_ghostObject.CreateBox(
				m_position,
				m_rotation,
				m_scale * m_ghostscale
			);
			m_hanteifin = true;
		}
	}
}