/// <summary>
/// �G��or�v���C���[�̍U����������Ă���N���X
/// </summary>
#include "stdafx.h"
#include "Wepon_ghost.h"
#include "gameObject/ysGameObjectManager.h"
#include "Player.h"
const float m_ghostscale = 200.0f;

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
	m_ghostTimer += 30.0f * GameTime().GetFrameDeltaTime();
	//�\������Ă��鎞�Ԃ�2.0���ȏ�Ȃ������
	if (m_ghostTimer > 2.0f)
	{
		m_ghostTimer = 0.0f;
		DeleteGO(this);
	}
}

void Wepon_ghost::GhostInit()
{
	//nullptr�Ȃ�
	//if (m_player != nullptr) {
	if(m_playerCall){
		if (!m_hanteifin) {
			m_ghostObject.CreateBox(
				m_player->GetPosition(),
				m_rotation,
				m_scale * m_ghostscale// *1.5f//1.2f//�v���C���[���������蔻���傫��
			);
		}
		m_hanteifin = true;
	}
	else {
		if (!m_hanteifin) {
			m_ghostObject.CreateBox(
				m_position,
				m_rotation,
				m_scale * m_ghostscale * 5.5f
			);
		}
		m_hanteifin = true;

	}
}