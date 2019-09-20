#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "BackGround.h"
#include "level/Level.h"
#include "gameObject/ysGameObjectManager.h"
//コンストラクタが呼ばれるとレベルでキャラを表示させるようにしている
Game::Game()
{
	//プレイヤー
	Player* player = g_goMgr.NewGameObject<Player>();
	BackGround* layer = g_goMgr.NewGameObject<BackGround>();
	Level level;
	level.Init(L"Assets/level/musou_stage.tkl",
		[&](const LevelObjectData& objdata) {
		//配置されているオブジェクトがasigaruならtrue
		//wcscmpは文字列の比較を行う関数。
			if (wcscmp(objdata.name, L"asigaru") == 0) {
				//インスタンスの作成
				Enemy* enemy = g_goMgr.NewGameObject<Enemy>();
				enemy->SetPos(objdata.position);
				enemy->SetPlayer(player);
				//可変長配列に↑のインスタンスを追加
				return true;
			}
		});
	//Enemy enemy = new Enemy;
	//m_Player->SetEnemy(enemy);
}

Game::~Game()
{

}

void Game::Update()
{
	
}