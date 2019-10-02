#pragma once
#include "gameObject/ysGameObject.h"
class Player;
class Enemy;
class BackGround;
class GameCamera;
class Game : public IGameObject
{
public:
	//コンストラクタ
	Game();
	//デストラクタ
	~Game();
	void Update();
private:
	//プレイヤー
	Player* m_player;
	Enemy* m_enemy;
	BackGround* m_backGround;
	GameCamera* m_gameCamera;
};

