#pragma once
#include "gameObject/ysGameObject.h"
#include "GameData.h"
#include "graphics/RenderTarget.h"
#include "graphics/Sprite.h"
#include "level/Level.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
class Fade;
class UI;
class Player;
class Enemy;
class BackGround;
class GameCamera;
class InGameSoundDirector;
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
	void GameDelete();
	//void ChangeRenderTarget(ID3D11DeviceContext* d3dDeviceContext, RenderTarget* renderTarget, D3D11_VIEWPORT* viewport);
	//void ChangeRenderTarget(ID3D11DeviceContext* d3dDeviceContext, ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStensil, D3D11_VIEWPORT* viewport);
	//void Render();
	//void ForwardRender();
	//void PostRender();
	//カメラの初期カー
	//void InitCamera();

private:
	//プレイヤー
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	BackGround* m_backGround = nullptr;
	GameCamera* m_gameCamera = nullptr;
	GameData* m_gamedata = nullptr;
	UI* m_ui;
	//RenderTarget m_renderTarget;				//メインレンダリングターゲット
	//Sprite m_copyMainRtToFrameBufferSprite;		//メインレンダリングに描かれた絵をフレームバッファにコピーするためのスプライト
	//D3D11_VIEWPORT m_frameBufferViewports;		//フレームバッファのビューポート
	//ID3D11RenderTargetView* m_frameBufferRenderTargetView = nullptr;	//フレームバッファのレンダリングターゲットビュー
	//ID3D11DepthStencilView* m_frameBufferDepthStencilView = nullptr;	//フレームバッファのデプスステンシルビュー
	Level m_level;
	Level m_level_EnemyBusyo;
	bool m_attack = false;
	bool m_isWireDraw = false;					//ワイヤーフレームを表示するかどうか。trueでついている
	bool m_gameDelete = false;					//ゲームから出る時にtrueになるようにする
	Fade* m_fade;
	CSoundEngine m_soundEngine;
	CSoundSource m_bgm;
	InGameSoundDirector* m_sd;
};