#pragma once
#include "graphics/Sprite.h"
#include "gameObject/ysGameObject.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
class Title : public IGameObject
{
public:
	Title();
	~Title();
	void Update();
	void Draw();
private:
	Sprite m_sprite;
	bool m_isWaitFadeOut = false;
	float m_timer = 0.0f;
	bool ando = false;

	CSoundEngine m_soundEngine;
	CSoundSource m_se;									//Œø‰Ê‰¹
	CSoundSource m_bgm;
};

