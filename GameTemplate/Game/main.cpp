#include "stdafx.h"
#include "system/system.h"
#include "level/Level.h"
#include "Title.h"
#include "gameObject/ysGameObjectManager.h"
#include "InGameSoundDirector.h"
#include "Fade.h"

const float NEARVALUE	 = 10.0f;		//カメラの近平面
const float FARVALUE	 = 100000.0f;	//カメラの遠平面
 
const CVector3 CAMERA_POSITION = { 0.0f, 100.0f, 300.0f };
const CVector3 CAMERA_TARGET = { 0.0f, 200.0f, 0.0f };

const CVector3 CAMERA_POSITION_2D = { 0.0f, 0.0f, -10.0f };
const CVector3 CAMERA_TARGET_2D = CVector3::Zero();

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
	g_camera3D.SetPosition(CAMERA_POSITION);
	g_camera3D.SetTarget(CAMERA_TARGET);

	g_camera2D.SetWidth(FRAME_BUFFER_W);
	g_camera2D.SetHeight(FRAME_BUFFER_H);
	g_camera2D.SetPosition(CAMERA_POSITION_2D);
	g_camera2D.SetTarget(CAMERA_TARGET_2D);
	g_camera3D.SetNear(NEARVALUE);
	g_camera3D.SetFar(FARVALUE);

	//g_camera2D.InitCamera();
	//タイトル
	Fade::Getinstance();

	g_goMgr.NewGameObject<Title>("Title");

	InGameSoundDirector::GetInstans();
	InGameSoundDirector::GetInstans().UpdateOn();

	CStopwatch sw;
	//ゲームループ。
	while (DispatchWindowMessage() == true)
	{
		//1フレームの時間計測を開始。
		sw.Start();


		//ゲームパッドの更新。	
		for (auto& pad : g_pad) {
			pad.Update();
		}
		
		//物理エンジンの更新。
		g_physics.Update();

		g_goMgr.Update();

		//g_graphicsEngine->RenderToShadowMap();
		g_graphicsEngine->Render();

		bool UpdateMode = InGameSoundDirector::GetInstans().GetisUpdateMode();
		if (UpdateMode) {
			InGameSoundDirector::GetInstans().Update();
		}

		//g_goMgr.Render();
		//g_goMgr.Draw();
		//g_goMgr.DrawNoAlphaBlend();
		
		//カメラの更新。
		g_camera3D.Update();
		g_camera2D.Update();
		Fade::Getinstance().Update();
		Fade::Getinstance().PostDraw();
	

		g_goMgr.Delete();


		//描画終了。
		g_graphicsEngine->EndRender();
		//1フレームの時間計測終了。
		sw.Stop();
	
		//1フレームの時間を記憶しておく。
		GameTime().PushFrameDeltaTime(sw.GetElapsed());
	}
}
