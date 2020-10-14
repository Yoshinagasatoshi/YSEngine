#include "stdafx.h"
#include "Sprite.h"




//頂点構造体。
struct Vertex {
	CVector4	pos;		//座標。
	CVector2	uv;			//UV座標。
};

Sprite::Sprite()
{
}


Sprite::~Sprite()
{
	if (m_vertexBuffer != nullptr) {
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}
	if (m_indexBuffer != nullptr) {
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}
	if (m_cb != nullptr) {
		m_cb->Release();
		m_cb = nullptr;
	}
	if (m_texture != nullptr) {
		m_texture->Release();
		m_texture = nullptr;
	}
	if (m_samplerState != nullptr) {
		m_samplerState->Release();
		m_samplerState = nullptr;
	}
	if (m_depthStencilState != nullptr) {
		m_depthStencilState->Release();
		m_depthStencilState = nullptr;
	}
	if (m_disableBlendState != nullptr) {
		m_disableBlendState->Release();
	}
}

void Sprite::Init(const wchar_t* textureFilePath, float w, float h)
{
	//共通の初期化処理を呼ぶ
	InitCommon(w,h);

	//テクスチャをロード。
	LoadTexture(textureFilePath);
}

void Sprite::Init(ID3D11ShaderResourceView* srv, float w, float h)
{
	//共通の初期化処理を呼ぶ
	InitCommon(w,h);

	m_texture = srv;
	if (m_texture != nullptr) {
		m_texture->AddRef();
	}
}

void Sprite::InitCommon(float w, float h)
{
	m_size.x = w;
	m_size.y = h;
	//頂点バッファの初期化
	InitVertexBuffer(w, h);
	//インデックスバッファの初期化
	InitIndexBuffer();
	//サンプラステートの初期化。
	InitSamplerState();
	//デプスステンシルステートの作成
	CreateDepthStencilState();
	//ブレンドステートの作成
	InitTranslucentBlendState();
	//シェーダーをロードする。
	m_vs.Load("Assets/shader/sprite.fx", "VSMain", Shader::EnType::VS);
	m_ps.Load("Assets/shader/sprite.fx", "PSMain", Shader::EnType::PS);
	//定数バッファを初期化
	InitConstantBuffer();
}

void Sprite::LoadShader()
{

}
void Sprite::InitConstantBuffer()
{
	//どんなバッファを作成するのかをせてbufferDescに設定する。
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;						//バッファで想定されている、読み込みおよび書き込み方法。
	desc.ByteWidth = (((sizeof(ConstantBuffer) - 1) / 16) + 1) * 16;	//定数バッファは16バイトアライメントになっている必要がある。
															//アライメントって→バッファのサイズが16の倍数ということです。
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//バッファをどのようなパイプラインにバインドするかを指定する。
															//定数バッファにバインドするので、D3D11_BIND_CONSTANT_BUFFERを指定する。
	desc.CPUAccessFlags = 0;								//CPU アクセスのフラグです。
															//CPUアクセスが不要な場合は0。
	//作成。
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	d3dDevice->CreateBuffer(&desc, NULL, &m_cb);
}
void Sprite::InitVertexBuffer(float w, float h)
{
	//上で定義した頂点を使用して頂点バッファを作成
	//頂点バッファを作成するためにはD3D11_BUFFER_DESCとD3D11_SUBEWSORCE_DATAを設定する必要がある。
	float halfW = w * 0.5f;
	float halfH = h * 0.5f;

	//頂点バッファの元データ。
	//引数で指定された縦、横のサイズの板ポリ。
	Vertex vertices[4] = {
		{
			CVector4(-halfW, -halfH, 0.0f, 1.0f),
			CVector2(0.0f, 1.0f),
		},
		{
			CVector4(halfW, -halfH, 0.0f, 1.0f),
			CVector2(1.0f, 1.0f),
		},
		{
			CVector4(-halfW, halfH, 0.0f, 1.0f),
			CVector2(0.0f, 0.0f)
		},
		{
			CVector4(halfW, halfH, 0.0f, 1.0f),
			CVector2(1.0f, 0.0f)
		}
	};

	//頂点バッファを作成するための情報を設定する。
	//頂点バッファを作成するためにはD3D11_BUFFER_DESCとD3D11_SUBRESOUECE_DATAを設定する。
	D3D11_BUFFER_DESC desc;							
	ZeroMemory(&desc, sizeof(desc));				//構造体のメンバを0で初期化する。
	desc.Usage = D3D11_USAGE_DEFAULT;				//バッファーで想定されている読み込み、および書き込みの方法

	desc.StructureByteStride = sizeof(Vertex);		//構造体バッファの１要素のサイズ。(頂点一個分のサイズです。)
	desc.ByteWidth = sizeof(Vertex) * 4;			//頂点バッファのサイズ。
													//四角形のポリゴンを描画するので、４頂点必要。
													//なので、一つの頂点のサイズ×４のサイズが必要となる。
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		//作成するバッファが頂点バッファであることを指定する。
	desc.CPUAccessFlags = 0;						//CPUから頂点バッファにアクセスするかのフラグ。
													//編集したりはしないので、０にしておく。

	//続いて、元データの情報を作る。
	//元データがないならこれはいらない。
	//今回は必要。
	D3D11_SUBRESOURCE_DATA initData;				
	ZeroMemory(&initData, sizeof(initData));		//構造体のメンバを0で初期化する。
	initData.pSysMem = vertices;					//元データのアドレスを代入。

	//ここまで設定してきた情報を使って、VRAM上に頂点バッファを作成する。
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	d3dDevice->CreateBuffer(&desc, &initData, &m_vertexBuffer);
}

void Sprite::InitIndexBuffer()
{
	//これがインデックス。頂点番号
	int index[6] = {
		0,1,2,		//三角形一つ目
		2,1,3		//三角形二つ目
	};

	//上で定義したインデックスを使用してインデックスバッファを作成する
	//インデックスバッファも頂点バッファと同じく
	//D3D11_BUFFER_DESCとD3D11_SUBRESOURCE_DATAを初期化する必要がある
	D3D11_BUFFER_DESC desc;							//構造体のメンバを0で初期化する。
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.StructureByteStride = 4;					//インデックス一個分のサイズ。shortなので2バイト。
	desc.ByteWidth = sizeof(index);				//インデックスバッファのサイズ。
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;		//作成するバッファがインデックスバッファであることを指定する。
	desc.CPUAccessFlags = 0;						//CPUから頂点バッファにアクセスするかのフラグ。
													//編集したりはしないので、０にしておく。

	D3D11_SUBRESOURCE_DATA initData = { 0 };			//構造体のメンバを0で初期化する。
	initData.pSysMem = index;						//元データのアドレスを代入する。

	//ここまで設定してきた情報を使って、VRAM上にインデックスバッファを作成する。
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	d3dDevice->CreateBuffer(&desc, &initData, &m_indexBuffer);
}

void Sprite::InitSamplerState()
{
	//テクスチャのサンプリング方法を指定するためのサンプラステートを作成。
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	//Clamp+バイリニアフィルタをかけるサンプラステートを作成する。
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	g_graphicsEngine->GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
}
void Sprite::LoadTexture(const wchar_t* textureFIlePath)
{
	//D3Dデバイスを取得。
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();

	DirectX::CreateDDSTextureFromFileEx(
		d3dDevice,
		textureFIlePath,
		0,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE,
		0,
		0,
		false,
		nullptr,
		&m_texture);
}

void Sprite::Update(const CVector3& trans, CQuaternion rot, CVector3 scale, CVector2 pivot)
{
	//ピボットを考慮に入れた平行移動行列を作成。
	//ピボットは真ん中が0.0, 0.0、左上が-1.0f, -1.0、右下が1.0、1.0になるようにする。
	CVector2 localPivot = pivot;
	localPivot.x -= 0.5f;
	localPivot.y -= 0.5f;
	localPivot.x *= -2.0f;
	localPivot.y *= -2.0f;
	//画像のハーフサイズを求める。
	CVector2 halfSize = m_size;
	halfSize.x *= 0.5f;
	halfSize.y *= 0.5f;
	CMatrix mPivotTrans;

	mPivotTrans.MakeTranslation(
		{ halfSize.x * localPivot.x, halfSize.y * localPivot.y, 0.0f }
	);
	//ワールド行列を計算する
	CMatrix mTrans, mRot, mScale;
	mTrans.MakeTranslation(trans);
	mRot.MakeRotationFromQuaternion(rot);
	mScale.MakeScaling(scale);
	m_world.Mul(mPivotTrans, mScale);
	m_world.Mul(m_world, mRot);
	m_world.Mul(m_world, mTrans);
}
void Sprite::DrawNoAlphaBlend()
{
	m_aiueo = i;
	//αブレンディングなしでドロー
	InternalDraw(m_disableBlendState);
}
void Sprite::Draw()
{
	m_aiueo = a;
	//こちらは半透明合成
	InternalDraw(m_translucentBlendState);
}
void Sprite::InternalDraw(ID3D11BlendState* blendState)
{
	//デバイスコンテキストを引っ張ってくる。
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();

	unsigned int vertexSize = sizeof(Vertex);	//頂点サイズ
	unsigned int offset = 0;
	//デプスステンシルステートを切り替える
	d3dDeviceContext->OMSetDepthStencilState(m_depthStencilState, 0);

	d3dDeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexSize, &offset);
	//インデックスバッファを設定。
	d3dDeviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//頂点シェーダーを設定。
	d3dDeviceContext->VSSetShader((ID3D11VertexShader*)m_vs.GetBody(), nullptr, 0);
	//ピクセルシェーダを設定。
	d3dDeviceContext->PSSetShader((ID3D11PixelShader*)m_ps.GetBody(), nullptr, 0);
	//頂点レイアウトを設定。
	d3dDeviceContext->IASetInputLayout(m_vs.GetInputLayout());
	//テクスチャを設定
	d3dDeviceContext->PSSetShaderResources(0, 1, &m_texture);
	//サンプラステートを設定
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
	//ブレンドステートを設定
	float blendFactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	d3dDeviceContext->OMSetBlendState(
		blendState,//設定するブレンディングステート
		blendFactor,
		0xffffffff
		);
	//ワールドビュープロジェクション行列を作成
	ConstantBuffer cb;
	cb.WVP = m_world;
	//2Dカメラ用行列も作成
	cb.WVP.Mul(cb.WVP,g_camera2D.GetViewMatrix());
	cb.WVP.Mul(cb.WVP,g_camera2D.GetProjectionMatrix());
	if (m_aiueo != i) {
		cb.alpha = m_alpha;
	}
	d3dDeviceContext->UpdateSubresource(m_cb, 0, NULL, &cb, 0, 0);
	d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
	d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cb);
	//プリミティブのトポロジーを設定。
	d3dDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//ここまで設定した内容でドロー
	d3dDeviceContext->DrawIndexed(6, 0, 0);
	
}



void Sprite::CreateDepthStencilState()
{
	//D3Dデバイスを取得。
	auto pd3d = g_graphicsEngine->GetD3DDevice();
	//作成する深度ステンシルステートの定義を設定していく。
	D3D11_DEPTH_STENCIL_DESC desc = { 0 };
	desc.DepthEnable = false;					 //Zテストが有効。
	//desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; //ZバッファにZ値を描き込む。
	//desc.DepthFunc = D3D11_COMPARISON_LESS;		 //Z値が小さければフレームバッファに描き込む。
	   //デプスステンシルステートを作成。
	pd3d->CreateDepthStencilState(&desc, &m_depthStencilState);
}

void Sprite::InitTranslucentBlendState()
{
	//作成するブレンドステートの情報を設定する。
	CD3D11_DEFAULT defaultSettings;
	//デフォルトセッティングで初期化する。
	CD3D11_BLEND_DESC blendDesc(defaultSettings);

	//αブレンディングを有効にする。
	blendDesc.RenderTarget[0].BlendEnable = true;

	//ソースカラーのブレンディング方法を指定している。
	//ソースカラーとはピクセルシェーダ―からの出力を指している。
	//この指定では、ソースカラーをSRC(rgba)とすると、
	//最終的なソースカラーは下記のように計算される。
	//最終的なソースカラー = SRC.rgb × SRC.a・・・・・・　①
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;

	//ディスティネーションカラーのブレンディング方法を指定している。
	//ディスティネーションカラーとは、
	//すでに描き込まれているレンダリングターゲットのカラーを指している。
	//この指定では、ディスティネーションカラーをDEST(rgba)、
	//ソースカラーをSRC(RGBA)とすると、最終的なディスティネーションカラーは
	//下記のように計算される。
	//最終的なディスティネーションカラー = DEST.rgb × (1.0f - SRC.a)・・・・・②
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	//最終的にレンダリングターゲットに描き込まれるカラーの計算方法を指定している。
	//この指定だと、①＋②のカラーが書き込まれる。
	//つまり、最終的にレンダリングターゲットに描き込まれるカラーは
	//SRC.rgb × SRC.a + DEST.rgb × (1.0f - SRC.a)
	//となる。
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	//この設定で、ブレンドステートを作成すると
	//半透明合成を行えるブレンドステートが作成できる。
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	d3dDevice->CreateBlendState(&blendDesc, &m_translucentBlendState);

	//αブレンディング向こうのステートを作成
	blendDesc.RenderTarget[0].BlendEnable = false;
	d3dDevice->CreateBlendState(&blendDesc, &m_disableBlendState);
}
