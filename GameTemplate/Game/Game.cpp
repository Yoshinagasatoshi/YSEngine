#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "Enemy_asigaru.h"
#include "Enemy_Busyo.h"
#include "Enemy_Bomber.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "GameData.h"
#include "UI.h"
#include "gameObject/ysGameObjectManager.h"
#include "Fade.h"

//倒された数の指標
//const int knockDownNum = 50;
//コンストラクタが呼ばれるとレベルでキャラを表示させるようにしている
Game::Game()
{
	g_goMgr.ResetCount();
	//InitCamera();
	//プレイヤー
	m_player = g_goMgr.NewGameObject<Player>("Player");
	m_player->SetGameInfo(this);
	m_ui = g_goMgr.NewGameObject<UI>("UI");
	m_ui->SetPlayerInfo(m_player);
	m_backGround = g_goMgr.NewGameObject<BackGround>("BackGround");
	m_gameCamera = g_goMgr.NewGameObject<GameCamera>("GameCamera");
	m_gameCamera->SetPlayerInfo(m_player);
	m_gamedata = g_goMgr.NewGameObject<GameData>("GameData");
	m_gamedata->SetPlayerInfo(m_player);
	bool isHoge = true;
	int enemyCount = 0;
	//レベルでモデルを出す。
	m_level.Init(L"Assets/level/musou_honkakustage.tkl",
		[&](const LevelObjectData& objdata) {
			//足軽
			//if (enemyCount < 20) {
				if (wcscmp(objdata.name, L"asigaru") == 0) {
				//	enemyCount++;
					isHoge = false;
					//インスタンスの作成
					m_enemy = g_goMgr.NewGameObject<Enemy_asigaru>("Enemy_asigaru");
					m_enemy->SetPos(objdata.position);
					//enemy->SetRot(objdata.rotation);
					m_enemy->SetPlayerInfo(m_player);
					m_enemy->SetGameCameraInfo(m_gameCamera);
					//可変長配列に↑のインスタンスを追加
					return true;
				}
			//}
			//return true;
			//if (wcscmp(objdata.name, L"enemy_busyo") == 0) {
			//	//インスタンスの作成
			//	m_enemy = g_goMgr.NewGameObject<Enemy_Busyo>("Enemy_busyo");
			//	m_enemy->SetPos(objdata.position);
			//	m_enemy->SetPlayerInfo(m_player);
			//	m_enemy->SetGameinfo(this);
			//	return true;
			//}
			if (wcscmp(objdata.name, L"asigaru_taicho") == 0) {
				//インスタンスの作成
				if (isHoge) {
					isHoge = false;
					m_enemy = g_goMgr.NewGameObject<Enemy_Bomber>("Enemy_bomber");
					m_enemy->SetPos(objdata.position);
					m_enemy->SetPlayerInfo(m_player);
					m_enemy->SetGameinfo(this);
				}
				return true;
			}
		});
	Fade::Getinstance().StartFadeOut();
	//メインとなるレンダリングターゲット
	//m_renderTarget.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R16G16B16A16_UNORM);
}

Game::~Game()
{
	m_player = nullptr;
	g_goMgr.DeleteGOObject(m_player);
	m_backGround = nullptr;
	g_goMgr.DeleteGOObject(m_backGround);
	m_ui = nullptr;
	g_goMgr.DeleteGOObject(m_ui);
	m_gameCamera = nullptr;
	g_goMgr.DeleteGOObject(m_gameCamera);
	m_gamedata = nullptr;
	g_goMgr.DeleteGOObject(m_gamedata);
	g_goMgr.FindGameObjects<Enemy>("Enemy_asigaru", [&](Enemy* enemy) {
		enemy = nullptr;
		g_goMgr.DeleteGOObject(enemy);
		return true;
		});
	g_goMgr.FindGameObjects<Enemy>("Enemy_busyo", [&](Enemy* enemy) {
		enemy = nullptr;
		g_goMgr.DeleteGOObject(enemy);
		return true;
		});
	g_goMgr.FindGameObjects<Enemy>("Enemy_bomber", [&](Enemy* enemy) {
		enemy = nullptr;
		g_goMgr.DeleteGOObject(enemy);
		return true;
		});
	g_goMgr.ResetCount();
}

void Game::Update()
{
	if (m_gameDelete) {
		m_gameDelete = false;
		g_goMgr.DeleteGOObject(this);
	}
}


void Game::Draw()
{
	/// <summary>
	/// render関数の処理をDraw関数に書いていたけど
	/// オーバーフローが起きたのでRenderに委託した。
	/// </summary>
}


void Game::GameDelete()
{
	m_gameDelete = true;
}
