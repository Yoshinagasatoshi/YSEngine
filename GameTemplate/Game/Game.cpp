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
#include "InGameSoundDirector.h"
#include "Fade.h"
//完全に一致であるか？
const int String_match = 0;	//文字列に違いはないかを確認するために使用する変数

//倒された数の指標
//const int knockDownNum = 50;
//コンストラクタが呼ばれるとレベルでキャラを表示させるようにしている
Game::Game()
{
	//オブジェクトのカウントをリセット
	g_goMgr.ResetCount();
	//無双ゲージのリセット
	g_goMgr.ResetMusouGauge();
	//音SE素材
	InGameSoundDirector::GetInstans().SoundInit();
	//InGameSoundDirector::GetInstans().InGameStartUpBGM();
	//InitCamera();
	//プレイヤーなどのインスタンスを作成
	m_player = g_goMgr.NewGameObject<Player>("Player");
	m_player->SetGameInfo(this);
	m_ui = g_goMgr.NewGameObject<UI>("UI");
	m_ui->SetPlayerInfo(m_player);
	m_ui->SetGameInfo(this);
	m_backGround = g_goMgr.NewGameObject<BackGround>("BackGround");
	m_gameCamera = g_goMgr.NewGameObject<GameCamera>("GameCamera");
	m_gameCamera->SetPlayerInfo(m_player);
	m_gamedata = g_goMgr.NewGameObject<GameData>("GameData");
	m_gamedata->SetPlayerInfo(m_player);
	int enemyCount = 0;//デバック用コード　後で消します
	//敵の総数は何体？数えます。
	g_goMgr.EnemyNumResetCount();
	//レベルでモデルを出す。
	m_level.Init(L"Assets/level/musou_honkakustage.tkl",
		[&](const LevelObjectData& objdata) {
			//足軽
				if (wcscmp(objdata.name, L"asigaru") == String_match) {
					g_goMgr.EnemyCounting();
					//インスタンスの作成
					m_enemy = g_goMgr.NewGameObject<Enemy_asigaru>("Enemy_asigaru");
					m_enemy->SetPos(objdata.position);
					//enemy->SetRot(objdata.rotation);
					m_enemy->SetPlayerInfo(m_player);
					m_enemy->SetGameCameraInfo(m_gameCamera);
					//可変長配列に↑のインスタンスを追加
					return true;
				}
				//敵武将
				if (wcscmp(objdata.name, L"enemy_busyo") == String_match) {
					//g_goMgr.EnemyCounting();
					//インスタンスの作成
					m_enemy = g_goMgr.NewGameObject<Enemy_Busyo>("Enemy_busyo");
					m_enemy->SetPos(objdata.position);
					m_enemy->SetPlayerInfo(m_player);
					m_enemy->SetGameinfo(this);
					return true;
				}
				//隊長兵(爆発兵)
				if (wcscmp(objdata.name, L"asigaru_taicho") == String_match) {
					//インスタンスの作成
					//g_goMgr.EnemyCounting();
					m_enemy = g_goMgr.NewGameObject<Enemy_Bomber>("Enemy_bomber");
					m_enemy->SetPos(objdata.position);
					m_enemy->SetPlayerInfo(m_player);
					m_enemy->SetGameinfo(this);
					return true;
				}
			return true;
		});
	Fade::Getinstance().StartFadeOut();
}

Game::~Game()
{
	g_goMgr.DeleteGOObject(m_player);
	g_goMgr.DeleteGOObject(m_backGround);
	g_goMgr.DeleteGOObject(m_ui);
	g_goMgr.DeleteGOObject(m_gameCamera);
	g_goMgr.DeleteGOObject(m_gamedata);
	//ちゃんと140回呼ばれている
	g_goMgr.FindGameObjects<Enemy>("Enemy_asigaru", [&](Enemy* enemy) {
		g_goMgr.DeleteGOObject(enemy);

		g_goMgr.EnemyNumSubtract();
		return true;
		});
	g_goMgr.FindGameObjects<Enemy>("Enemy_busyo", [&](Enemy* enemy) {
		g_goMgr.EnemyNumSubtract();

		g_goMgr.DeleteGOObject(enemy);
		return true;
		});
	g_goMgr.FindGameObjects<Enemy>("Enemy_bomber", [&](Enemy* enemy) {
		g_goMgr.EnemyNumSubtract();

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
}


void Game::GameDelete()
{
	//ゲームそのもののデリートフラグ。
	//ゲームクリアとかゲームオーバーで
	//シーンを転換するときに呼ばれます。
	m_gameDelete = true;
}
