#pragma once
#include "gameObject/ysGameObjectManager.h"
class Fade
{
public:

	//�ǂ��ł��A�N�Z�X�ł���悤�ɂ������̂�
	//�V���O���g���ɂ��Ă݂�B

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

	//���̊֐����Ă΂ꂽ��A���X�ɖ��邭�Ȃ�B
	void StartFadeIn()
	{
		m_state = enState_FadeIn;
	}
	//���̊֐����Ă΂ꂽ��A���X�ɈÂ��Ȃ�B
	void StartFadeOut()
	{
		m_state = enState_FadeOut;
	}
	//���̃t�F�[�h�̏�Ԃ́H
	bool IsFade() const 
	{
		return m_isFade;
	}
	//�Ō�܂ŏ������I������H
	bool IsEnd()const 
	{
		return m_isEnd;
	}
	//���݂̃��l����肷��B
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
	enState_Fade m_state = enState_Idle;//��ԑJ�ځB
	Sprite m_sprite;					//�摜�f�[�^�B���[�f�B���O��ʗp
	float m_currentAlpha = 0.5f;		//���l�B
	bool m_isFade = true;				//�t�F�[�h���Ă��邩�̃t���O
	bool m_isEnd = false;				//�t�F�[�h���I��������̃t���O
};