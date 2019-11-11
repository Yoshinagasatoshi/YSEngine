#pragma once
#include "gameObject/ysGameObject.h"
#include "GameData.h"
//class UI;
class Player;
class Enemy;
class BackGround;
class GameCamera;
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
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	BackGround* m_backGround = nullptr;
	GameCamera* m_gameCamera = nullptr;
	GameData* m_gamedata = nullptr;
//	UI* m_ui;
};

