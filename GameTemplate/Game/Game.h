#pragma once
#include "gameObject/ysGameObject.h"
class Player;
class Enemy;
class BackGround;
class Game : public IGameObject
{
public:
	//�R���X�g���N�^
	Game();
	//�f�X�g���N�^
	~Game();
	void Update();
private:
	//�v���C���[
	Player* m_player;
	Enemy* m_enemy;
	BackGround* m_backGround;
};

