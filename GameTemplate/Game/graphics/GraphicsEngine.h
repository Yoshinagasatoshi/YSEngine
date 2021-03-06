#pragma once
#include "graphics/RenderTarget.h"
#include "graphics/Sprite.h"

/// <summary>
 /// レンダリングモード。
 /// </summary>
enum EnRenderMode {
	enRenderMode_Invalid,			//不正なレンダリングモード。
	enRenderMode_CreateCascadeShadowMap,    //カスケードシャドウマップ生成
	enRenderMode_CreateShadowMap,	//シャドウマップ生成。
	enRenderMode_Normal,			//通常レンダリング。
	enRenderMode_Num,				//レンダリングモードの数。
};

class ShadowMap;
class CascadeShadowMap;
class PostEffect;

/*!
 *@brief	グラフィックスエンジン。
 */
class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();
	/*!
	 *@brief	初期化。
	 *@param[in]	hWnd		ウィンドウハンドル。
	 */
	void Init(HWND hWnd);
	/*!
	 *@brief	解放。
	 */
	void Release();
	/*!
	 *@brief	D3D11デバイスを取得。
	 */
	ID3D11Device* GetD3DDevice()
	{
		return m_pd3dDevice;
	}
	/*!
	 *@brief	D3D11デバイスコンテキストを取得。
	 */
	ID3D11DeviceContext* GetD3DDeviceContext()
	{
		return m_pd3dDeviceContext;
	}
	/// <summary>
	/// シャドウマップを取得
	/// </summary>
	/// <returns></returns>
	ShadowMap* GetShadowMap()
	{
		return m_shadowMap;
	}
	/// <summary>
	/// カスケードシャドウマップを取得
	/// </summary>
	/// <returns></returns>
	CascadeShadowMap* GetCascadeShadowMap()
	{
		return m_cascadeShadowMap;
	}

	/// <summary>
	/// スプライトバッチの取得
	/// </summary>
	/// <returns>スプライトバッチ</returns>
	DirectX::SpriteBatch* GetSpriteBatch()
	{
		return m_spriteBatch.get();
	}

	/// <summary>
	/// スプライトフォントの取得
	/// </summary>
	/// <returns>スプライトフォント</returns>
	DirectX::SpriteFont* GetSpriteFont()
	{
		return m_spriteFont.get();
	}


	RenderTarget* GetMainRenderTarget()
	{
		return &m_renderTarget;
	}
	/*!
	 *@brief	描画開始。
	 */
	void BegineRender();
	/*!
	 *@brief	描画終了。
	 */
	void EndRender();
	void ChangeRenderTarget(ID3D11DeviceContext* d3dDeviceContext, RenderTarget* renderTarget, D3D11_VIEWPORT* viewport);
	void ChangeRenderTarget(ID3D11DeviceContext* d3dDeviceContext, ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStensil, D3D11_VIEWPORT* viewport);
	void ChangeRenderTarget(RenderTarget* renderTarget, D3D11_VIEWPORT* viewport);
	void Render();
	void ForwardRender();
	void PostRender();
	/// <summary>
	/// シャドウマップを生成
	/// </summary>
	void RenderToShadowMap();
private:
	D3D_FEATURE_LEVEL		m_featureLevel;				//Direct3D デバイスのターゲットとなる機能セット。
	ID3D11Device*			m_pd3dDevice = NULL;		//D3D11デバイス。
	IDXGISwapChain*			m_pSwapChain = NULL;		//スワップチェイン。
	ID3D11DeviceContext*	m_pd3dDeviceContext = NULL;	//D3D11デバイスコンテキスト。
	ID3D11RenderTargetView* m_backBuffer = NULL;		//バックバッファ。
	ID3D11RasterizerState*	m_rasterizerState = NULL;	//ラスタライザステート。
	ID3D11Texture2D*		m_depthStencil = NULL;		//デプスステンシル。
	ID3D11DepthStencilView* m_depthStencilView = NULL;	//デプスステンシルビュー。

	RenderTarget m_renderTarget;				//メインレンダリングターゲット
	Sprite m_copyMainRtToFrameBufferSprite;		//メインレンダリングに描かれた絵をフレームバッファにコピーするためのスプライト
	D3D11_VIEWPORT m_frameBufferViewports;		//フレームバッファのビューポート
	ID3D11RenderTargetView* m_frameBufferRenderTargetView = nullptr;	//フレームバッファのレンダリングターゲットビュー
	ID3D11DepthStencilView* m_frameBufferDepthStencilView = nullptr;	//フレームバッファのデプスステンシルビュー

	bool m_isWireDraw = false;					//ワイヤーフレームを表示するかどうか。trueでついている
	CD3D11_VIEWPORT m_viewPort;
	ShadowMap* m_shadowMap = nullptr;
	CascadeShadowMap* m_cascadeShadowMap = nullptr;
	PostEffect* m_postEffect = nullptr;

	//定義。
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;     //
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;		 //
};

extern GraphicsEngine* g_graphicsEngine;			//グラフィックスエンジン