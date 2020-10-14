#include "stdafx.h"
#include "Fade.h"

Fade::Fade()
{
	//フェード用の画像を用意
	m_sprite.Init(L"Assets/sprite/fade.dds",1280.0f, 720.0f);
	m_sprite.SetAlpha(m_currentAlpha);
}

Fade::~Fade()
{

}

void Fade::Update()
{
	switch (m_state) {
		//a値が1以下なら濃くしていく
	case enState_FadeIn:
		if (m_currentAlpha < 1.0f) {
			m_currentAlpha += 0.03f;
			m_isFade = true;
			m_isEnd = false;
		}
		else {
			m_currentAlpha = 1.0f;
			m_isFade = false;
			m_state = enState_Idle;
		}
		break;
	case enState_FadeOut:
		//a値が0以上なら薄くしていく
		if (m_currentAlpha >= 0.0f) {
			m_currentAlpha -= 0.03f;
			m_isFade = true;
			m_isEnd = false;
		}
		else {
			m_currentAlpha = 0.0f;
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
