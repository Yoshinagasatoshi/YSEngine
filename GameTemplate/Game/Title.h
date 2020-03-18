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
};

