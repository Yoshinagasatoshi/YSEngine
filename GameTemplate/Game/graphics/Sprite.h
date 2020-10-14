#pragma once
#include <algorithm>
#include "graphics/shader.h"
/// <summary>
/// 現在のスプライトクラス
/// <remarks>
/// 画像を張り付けることができる
/// </remarks>
/// </summary>
class Sprite {
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	Sprite::Sprite();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	Sprite::~Sprite();
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureFilePath">テクスチャ</param>
	/// <param name="w">画像の幅</param>
	/// <param name="h">画像の高さ</param>
	void Init(const wchar_t* textureFilePath, float w, float h);
	/// <summary>
	/// テクスチャのSRVを指定して初期化
	/// </summary>
	/// <param name="srv"></param>
	/// <param name="w"></param>
	/// <param name="h"></param>
	void Init(ID3D11ShaderResourceView* srv, float w, float h);
	/// <summary>
	/// 初期化共通処理
	/// </summary>
	/// <param name="w"></param>
	/// <param name="h"></param>
	void InitCommon(float w , float h);
	/// <summary>
	/// ワールド行列を更新。
	/// </summary>
	/// <param name="trans">平行移動量</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡大率</param>
	/// <param name="pivot"></param>
	/// ピボット...画像の基点
	/// 0.5,0.5で画像の真ん中
	/// 0.0,0.0で画像の左下
	/// 1.0,1.0で画像の右上
	void Update(const CVector3& trans, CQuaternion rot, CVector3 scale, CVector2 pivot = { 0.5f,0.5f });
	/// <summary>
	/// 描画。
	/// </summary>
	/// <param name="mView">カメラ行列</param>
	/// /// <param name="mView">プロジェクション行列</param>
	void Draw();
	/// <summary>
	/// 場所をセッティング
	/// </summary>
	/// <param name="pos"></param>
	/// <summary>
	/// αブレンディングなしでドロー
	/// </summary>
	void DrawNoAlphaBlend();
	
	/// <param name="pos"></param>
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// 角度をセッティング
	/// </summary>
	/// <param name="rot"></param>
	void SetRotation(CQuaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// 大きさをセッティング
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(CVector3 scale)
	{
		m_scale = scale;
	}
	/*!
* @brief	乗算カラーを設定。
*@param[in]	mulColor	乗算カラー。
*/
	void SetMulColor(const CVector4& mulColor)
	{
		m_mulColor = mulColor;
	}
	void SetAlpha(float alpha)
	{
		m_alpha = alpha;
	}
private:
	void InternalDraw(ID3D11BlendState* blendState);
	/// <summary>
	/// シェーダーをロード。
	/// </summary>
	void LoadShader();
	/// <summary>
	/// 頂点バッファを初期化する。
	/// </summary>
	void InitVertexBuffer(float w, float h);
	/// <summary>
	/// インデックスバッファを作成。
	/// </summary>
	void InitIndexBuffer();
	/// <summary>
	/// サンプラステートを作成。
	/// </summary>
	void InitSamplerState();
	/// <summary>
	/// 定数バッファの作成。
	/// </summary>
	void InitConstantBuffer();
	/// <summary>
	/// テクスチャをロ―ド。
	/// </summary>
	/// <param name="textureFIlePath">ロードするテクスチャのファイルパス。</param>
	void LoadTexture(const wchar_t* textureFIlePath);
	void InitTranslucentBlendState();
	void CreateDepthStencilState();

private:
	struct ConstantBuffer {
		CMatrix WVP;  //WorldViewProjection(行列)の略
		float alpha;
	};
	ID3D11Buffer* m_vertexBuffer = nullptr;	//VRAM上の頂点バッファにアクセスするためのインターフェース。
	ID3D11Buffer* m_indexBuffer = nullptr;		//VRAM上のインデックスバッファにアクセスするためのインターフェース。
	ID3D11Buffer* m_cb = nullptr;			//GPU側の定数バッファにアクセスするためのインターフェース。
	ID3D11ShaderResourceView* m_texture = nullptr;	//テクスチャにアクセスするためのインターフェース。
	ID3D11SamplerState* m_samplerState = nullptr;	//サンプラステート。
	ID3D11BlendState* m_translucentBlendState = nullptr;
	ID3D11DepthStencilState* m_depthStencilState = nullptr;
	ID3D11BlendState* m_disableBlendState = nullptr;
	Shader	m_vs;											//頂点シェーダー。
	Shader	m_ps;											//ピクセルシェーダー。
	CMatrix m_world = CMatrix::Identity();					//ワールド行列。
	CVector3 m_position = CVector3::Zero();		
	CQuaternion m_rotation = CQuaternion::Identity();
	CVector3 m_scale = CVector3::One();
	CVector2 m_size = CVector2::Zero();
	float m_alpha = 1.0f;
	CVector4 m_mulColor = CVector4::White();
	
	enum state {
		a = 0,
		i,
		num
	};
	state m_aiueo = num;
};