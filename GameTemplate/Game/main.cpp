#include "stdafx.h"
#include "system/system.h"
#include "level/Level.h"
#include "Title.h"
//#include "gameObject/ysGameObjectManager.h"
#include "Fade.h"

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");
	
	//サウンドはゲームじゃ…
	//サウンドエンジンを初期化
	
	g_goMgr.InitEffekseer();
	
	//カメラを初期化。
	g_camera3D.InitCamera();
	g_camera3D.SetPosition({ 0.0f, 100.0f, 300.0f });
	g_camera3D.SetTarget({ 0.0f, 200.0f, 0.0f });

	g_camera2D.SetWidth(FRAME_BUFFER_W);
	g_camera2D.SetHeight(FRAME_BUFFER_H);
	g_camera2D.SetPosition({ 0.0f, 0.0f, -10.0f });
	g_camera2D.SetTarget(CVector3::Zero());
	g_camera3D.SetNear(10.0f);
	g_camera3D.SetFar(100000.0f);

	//g_camera2D.InitCamera();
	//タイトル
	Fade::Getinstance();

	Title* title = g_goMgr.NewGameObject<Title>("Title");

	//ゲームループ。
	while (DispatchWindowMessage() == true)
	{
		
		//ゲームパッドの更新。	
		for (auto& pad : g_pad) {
			pad.Update();
		}
		
		//物理エンジンの更新。
		g_physics.Update();

		g_goMgr.Update();
		g_goMgr.Render();
		g_goMgr.Draw();
		g_graphicsEngine->Render();
		Fade::Getinstance().Update();
		Fade::Getinstance().PostDraw();
		
		//カメラの更新。
		g_camera3D.Update();
		g_camera2D.Update();
		//描画終了。
		g_graphicsEngine->EndRender();
	}
}
