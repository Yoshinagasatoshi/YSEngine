#pragma once
#include "gameObject/ysGameObject.h"
#include "GameData.h"
#include "graphics/RenderTarget.h"
//class UI;
class Player;
class Enemy;
class BackGround;
class GameCamera;
class Game : public IGameObject
{
public:
	enum EnRenderMode {
		enRenderMode_NORMAL,
		enRenderMode_Silhouette,
		enRenderMode_Num
	};
	//�R���X�g���N�^
	Game();
	//�f�X�g���N�^
	~Game();
	void Update();
	//�������牺�̏����̓N���X������Ĉϑ�����B
	void Draw();
	void Render();
private:
	//�v���C���[
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	BackGround* m_backGround = nullptr;
	GameCamera* m_gameCamera = nullptr;
	GameData* m_gamedata = nullptr;
//	UI* m_ui;
	//�f�v�X�X�e���V���r���[��
	RenderTarget m_renderTarget;
};

