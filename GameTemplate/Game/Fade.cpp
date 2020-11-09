#include "stdafx.h"
#include "Fade.h"

const float FEEDIN_VALUE = 1.0f;	//フェードイン
const float FEEDAUTO_VALUE = 0.0f;	//フェードアウト
const float ADD_VAL = 0.03f;		//追加される数値群

Fade::Fade()
{
	//フェード用の画像を用意
	m_sprite.Init(L"Assets/sprite/fade.dds", FRAME_BUFFER_W, FRAME_BUFFER_H);
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
		//a値が0以上なら薄くしていく
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
