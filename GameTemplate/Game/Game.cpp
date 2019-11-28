#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "Enemy_asigaru.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "level/Level.h"
#include "GameData.h"
//#include "UI.h"
#include "gameObject/ysGameObjectManager.h"
//コンストラクタが呼ばれるとレベルでキャラを表示させるようにしている
Game::Game()
{
	//プレイヤー
	m_player = g_goMgr.NewGameObject<Player>();
	m_backGround = g_goMgr.NewGameObject<BackGround>();
	m_gameCamera = g_goMgr.NewGameObject<GameCamera>();
	m_gamedata = g_goMgr.NewGameObject<GameData>();
//	m_ui = g_goMgr.NewGameObject<UI>();
	m_gameCamera->SetPlayerInfo(m_player);
	m_gamedata->SetPlayerInfo(m_player);
	Level level;
	level.Init(L"Assets/level/musou_stage.tkl",
		[&](const LevelObjectData& objdata) {
		//配置されているオブジェクトがasigaruならtrue
		//wcscmpは文字列の比較を行う関数。
			if (wcscmp(objdata.name, L"asigaru") == 0) {
				//インスタンスの作成
				m_enemy = g_goMgr.NewGameObject<Enemy_asigaru>();
				m_enemy->SetPos(objdata.position);
				//enemy->SetRot(objdata.rotation);
				m_enemy->SetPlayerInfo(m_player);
				//可変長配列に↑のインスタンスを追加
				return true;
			}
		});
	//メインとなるレンダリングターゲット
	m_renderTarget.Create(1280, 720, DXGI_FORMAT_R16G16B16A16_UNORM);

	//↑に描かれた絵を
	//フレームバッファにコピーするためのスプライトの初期化
	//スプライトクラスがねえじゃーん。作ります
	//Enemy enemy = new Enemy;
	//m_Player->SetEnemy(enemy);
}

Game::~Game()
{
	g_goMgr.DeleteGOObject(m_player);
	g_goMgr.DeleteGOObject(m_backGround);
}

void Game::Update()
{
	Render();
}

void Game::Draw()
{
	/// <summary>
	/// render関数の処理をDraw関数に書いていたけど
	/// オーバーフローが起きたので委託した。
	/// </summary>
}

void Game::Render()
{
	//レンダリングモード。
	EnRenderMode renderMode = enRenderMode_NORMAL;
	//描画開始
	g_graphicsEngine->BegineRender();
	/// <summary>
	/// オフスクリーンレンタリング
	/// </summary>
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	//現在のレンダリングターゲットをバックアップしておく。
	ID3D11RenderTargetView* oldRenderTargetView;
	ID3D11DepthStencilView* oldDepthStencilView;
	d3dDeviceContext->OMGetRenderTargets(1, &oldRenderTargetView, &oldDepthStencilView);
	//レンダリングターゲットを切り替える。
	ID3D11RenderTargetView* rts[] = {
		m_renderTarget.GetRenderTargetView()
	};
	d3dDeviceContext->OMSetRenderTargets(1, rts, m_renderTarget.GetDepthStensilView());
	//レンダリングターゲットをクリア。
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; //red,green,blue,alpha
	m_renderTarget.ClearRenderTarget(clearColor);
	g_goMgr.Draw();
	////背景を描画。
	//m_backGround->Draw();
	////通常レンダリング。
	//m_player->Draw();
	//m_enemy->Draw();
	/// <summary>
	/// ここからオンスクリーンレンタリング
	/// </summary>
	//レンダリングターゲットを基に戻す。
	d3dDeviceContext->OMSetRenderTargets(1, &oldRenderTargetView, oldDepthStencilView);
	//レンダリングターゲットとデプスステンシルの参照を下す
	oldRenderTargetView->Release();
	oldDepthStencilView->Release();

	g_goMgr.Draw();
	//m_backGround->Draw();
	//m_player->Draw();
	//m_enemy->Draw();

	g_graphicsEngine->EndRender();
}