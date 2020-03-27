#pragma once
#include "graphics/Sprite.h"
#include "gameObject/ysGameObject.h"
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
	float m_targetTime = 5.0f;
	bool ando = false;
};

