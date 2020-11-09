#include "stdafx.h"
#include "Fade.h"

const float FEEDIN_VALUE = 1.0f;	//�t�F�[�h�C��
const float FEEDAUTO_VALUE = 0.0f;	//�t�F�[�h�A�E�g
const float ADD_VAL = 0.03f;		//�ǉ�����鐔�l�Q

Fade::Fade()
{
	//�t�F�[�h�p�̉摜��p��
	m_sprite.Init(L"Assets/sprite/fade.dds", FRAME_BUFFER_W, FRAME_BUFFER_H);
	m_sprite.SetAlpha(m_currentAlpha);
}

Fade::~Fade()
{

}

void Fade::Update()
{
	switch (m_state) {
		//a�l��1�ȉ��Ȃ�Z�����Ă���
	case enState_FadeIn:
		if (m_currentAlpha < FEEDIN_VALUE) {
			m_currentAlpha += ADD_VAL;
			m_isFade = true;
			m_isEnd = false;
		}
		else {
			m_currentAlpha = FEEDIN_VALUE;
			m_isFade = false;
			m_state = enState_Idle;
		}
		break;
	case enState_FadeOut:
		//a�l��0�ȏ�Ȃ甖�����Ă���
		if (m_currentAlpha >= FEEDAUTO_VALUE) {
			m_currentAlpha -= ADD_VAL;
			m_isFade = true;
			m_isEnd = false;
		}
		else {
			m_currentAlpha = FEEDAUTO_VALUE;
			m_isEnd = true;
			m_state = enState_Idle;
		}
		break;
	case enState_Idle:
		break;
	}
	m_sprite.SetAlpha(m_currentAlpha);
}

void Fade::PostDraw()
{
	m_sprite.Draw();
}
