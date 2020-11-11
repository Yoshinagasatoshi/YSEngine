#pragma once
#include "gameObject/ysGameObjectManager.h"
class Fade
{
public:

	//どこでもアクセスできるようにしたいので
	//シングルトンにしてみる。

	static Fade& Getinstance()
	{
		static Fade instance;
		return instance;
	}
private:
	Fade();
	~Fade();
public:
	void Update();
	void PostDraw();

	//この関数が呼ばれた後、徐々に明るくなる。
	void StartFadeIn()
	{
		m_state = enState_FadeIn;
	}
	//この関数が呼ばれた後、徐々に暗くなる。
	void StartFadeOut()
	{
		m_state = enState_FadeOut;
	}
	//今のフェードの状態は？
	bool IsFade() const 
	{
		return m_isFade;
	}
	//最後まで処理が終わった？
	bool IsEnd()const 
	{
		return m_isEnd;
	}
	//現在のα値を入手する。
	float GetAlpha()
	{
		return m_currentAlpha;
	}
private:
	enum enState_Fade {
		enState_FadeIn,
		enState_FadeOut,
		enState_Idle
	};
	enState_Fade m_state = enState_Idle;//状態遷移。
	Sprite m_sprite;					//画像データ。ローディング画面用
	float m_currentAlpha = 0.5f;		//α値。
	bool m_isFade = true;				//フェードしているかのフラグ
	bool m_isEnd = false;				//フェードが終わったかのフラグ
};