#pragma once
#include "graphics/Sprite.h"
#include "gameObject/ysGameObject.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
/// <summary>
/// �^�C�g���N���X
/// �{�^�����������ƃV�[���J�ڂ��s����V���v���ȃN���X�ł�
/// </summary>
class Title : public IGameObject
{
public:
	Title();
	~Title();
	void Update();
	void Draw();
private:
	Sprite m_sprite;									//�摜�f�[�^
	bool m_isWaitFadeOut = false;						//�t�F�[�h�A�E�g�������������ǂ���

	CSoundEngine m_soundEngine;							//�^�C�g���ōĐ����邽�߂̃T�E���h�G���W��
	CSoundSource m_se;									//���ʉ�
	CSoundSource m_bgm;									//�^�C�g���ŗ����a�f�l
};

