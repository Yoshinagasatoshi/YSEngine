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
	//コンストラクタ
	Game();
	//デストラクタ
	~Game();
	void Update();
	//ここから下の処理はクラスを作って委託する。
	void Draw();
	void Render();
private:
	//プレイヤー
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	BackGround* m_backGround = nullptr;
	GameCamera* m_gameCamera = nullptr;
	GameData* m_gamedata = nullptr;
//	UI* m_ui;
	//デプスステンシルビューを
	RenderTarget m_renderTarget;
};

