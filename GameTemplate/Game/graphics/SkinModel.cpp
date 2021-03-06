#include "stdafx.h"
#include "SkinModel.h"
#include "SkinModelDataManager.h"
#include "shadow\ShadowMap.h"


SkinModel::SkinModel()
{
	
}
SkinModel::~SkinModel()
{
	if (m_cb != nullptr) {
		//定数バッファを解放。
		m_cb->Release();
	}
	if (m_lightCb != nullptr) {
		//ライト定数バッファを開放
		m_lightCb->Release();
	}
	if (m_samplerState != nullptr) {
		//サンプラステートを解放。
		m_samplerState->Release();
	}
	if (m_albedoTextureSRV != nullptr) {
		//アルベドテクスチャを開放。
		m_albedoTextureSRV->Release();
	}
}
void SkinModel::Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis)
{
	//スケルトンのデータを読み込む。
	InitSkeleton(filePath);

	//定数バッファの作成。
	InitConstantBuffer();

	//ディレクションライトの初期化
	InitDirectionLight();

	//サンプラステートの初期化。
	InitSamplerState();

	//アルベドテクスチャの初期化。
	InitAlbedoTexture();

	//SkinModelDataManagerを使用してCMOファイルのロード。
	m_modelDx = g_skinModelDataManager.Load(filePath, m_skeleton);

	FindMesh([&](auto& mesh) {
		auto vMax = CVector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		auto vMin = CVector3(FLT_MAX, FLT_MAX, FLT_MAX);
		auto deviceContext = g_graphicsEngine->GetD3DDeviceContext();
		{
			D3D11_MAPPED_SUBRESOURCE subresource;
			auto hr = deviceContext->Map(mesh->vertexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subresource);
			if (FAILED(hr)) {
				return;
			}
			D3D11_BUFFER_DESC bufferDesc;
			mesh->vertexBuffer->GetDesc(&bufferDesc);
			auto vertexCount = (int)(bufferDesc.ByteWidth / mesh->vertexStride);
			auto pData = reinterpret_cast<char*>(subresource.pData);
			for (int i = 0; i < vertexCount; i++) {
				auto pos = *reinterpret_cast<CVector3*>(pData);
				vMax.Max(pos);
				vMin.Min(pos);
				//次の頂点へ。
				pData += mesh->vertexStride;
			}
			//頂点バッファをアンロック
			deviceContext->Unmap(mesh->vertexBuffer.Get(), 0);
		}

		auto halfSize = (vMax - vMin) * 0.5f;
		m_box.Init(halfSize);
		});

	m_enFbxUpAxis = enFbxUpAxis;
}
void SkinModel::InitSkeleton(const wchar_t* filePath)
{
	//スケルトンのデータを読み込む。
	//cmoファイルの拡張子をtksに変更する。
	std::wstring skeletonFilePath = filePath;
	//文字列から.cmoファイル始まる場所を検索。
	int pos = (int)skeletonFilePath.find(L".cmo");
	//.cmoファイルを.tksに置き換える。
	skeletonFilePath.replace(pos, 4, L".tks");
	//tksファイルをロードする。
	bool result = m_skeleton.Load(skeletonFilePath.c_str());
	if ( result == false ) {
		//スケルトンが読み込みに失敗した。
		//アニメーションしないモデルは、スケルトンが不要なので
		//読み込みに失敗することはあるので、ログ出力だけにしておく。
#ifdef _DEBUG
		char message[256];
		sprintf(message, "tksファイルの読み込みに失敗しました。%ls\n", skeletonFilePath.c_str());
		OutputDebugStringA(message);
#endif
	}
}
void SkinModel::InitConstantBuffer()
{
	//作成するバッファのサイズをsizeof演算子で求める。
	int bufferSize = sizeof(SVSConstantBuffer);
	//どんなバッファを作成するのかをせてbufferDescに設定する。
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));				//０でクリア。
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//バッファで想定されている、読み込みおよび書き込み方法。
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//バッファは16バイトアライメントになっている必要がある。
																//アライメントって→バッファのサイズが16の倍数ということです。
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//バッファをどのようなパイプラインにバインドするかを指定する。
																//定数バッファにバインドするので、D3D11_BIND_CONSTANT_BUFFERを指定する。
	bufferDesc.CPUAccessFlags = 0;								//CPU アクセスのフラグです。
																//CPUアクセスが不要な場合は0。
	//作成。
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_cb);
	//続いて、ライト用の定数バッファを作成。
	//作成するバッファのサイズを変更するだけ。
	bufferDesc.ByteWidth = sizeof(SLight);				//SDirectionLightは16byteの倍数になっているので、切り上げはやらない。
	bufferDesc.ByteWidth = (((bufferDesc.ByteWidth - 1) / 16) + 1) * 16;
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_lightCb);
}
void SkinModel::InitSamplerState()
{
	//テクスチャのサンプリング方法を指定するためのサンプラステートを作成。
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	g_graphicsEngine->GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
}
void SkinModel::InitDirectionLight()
{
	//とりあえず４本ディレクションライトを初期化
	m_light.directionLight.direction[0] = { 0.577f, -0.577f, 0.577f, 0.0f };
	m_light.directionLight.color[0] = { 0.5f, 0.5f, 0.5f, 1.0f };

	m_light.directionLight.direction[1] = { -0.707f, -0.707f, 0.0f, 0.0f };
	m_light.directionLight.color[1] = { 0.3f, 0.3f, 0.3f, 1.0f };

	m_light.directionLight.direction[2] = { 0.0f, -0.707f, -0.707f, 0.0f };
	m_light.directionLight.color[2] = { 0.3f, 0.3f, 0.3f, 1.0f };

	m_light.directionLight.direction[3] = { 1.0f, 0.0f, -1.0f, 0.0f };
	m_light.directionLight.color[3] = { 0.3f, 0.3f, 0.3f, 1.0f };

	m_light.specPow = 10.0f;
	m_light.AmbLight = { 0.8f,0.8f,0.8f };
}

void SkinModel::InitAlbedoTexture()
{
	//ファイル名を使って、テクスチャをロードして、ShaderResouceViewを作成する。
	HRESULT hr = DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(),
		L"Assets/modelData/_Users_GC1831_Desktop_photos_yari.png.dds",
		0,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE,
		0,
		0,
		false,
		nullptr,
		&m_albedoTextureSRV
	);
}
//ここを読んだら光が回りだす。
//特に意味はない
void SkinModel::Update()
{
	//ライトを回す。
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY(), 0.2f);
	for (int i = 0; i < Lightnumber; i++) {
		qRot.Multiply(m_light.directionLight.direction[i]);
	}
}

void SkinModel::UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale)
{
	//Update();
	//3dsMaxと軸を合わせるためのバイアス。
	CMatrix mBias = CMatrix::Identity();
	if (m_enFbxUpAxis == enFbxUpAxisZ) {
		//Z-up
		mBias.MakeRotationX(CMath::PI * -0.5f);
	}
	CMatrix transMatrix, rotMatrix, scaleMatrix;
	//平行移動行列を作成する。
	transMatrix.MakeTranslation( position );
	//回転行列を作成する。
	rotMatrix.MakeRotationFromQuaternion( rotation );
	rotMatrix.Mul(mBias, rotMatrix);
	//拡大行列を作成する。
	scaleMatrix.MakeScaling(scale);
	//ワールド行列を作成する。
	//拡大×回転×平行移動の順番で乗算するように！
	//順番を間違えたら結果が変わるよ。
	m_worldMatrix.Mul(scaleMatrix, rotMatrix);
	m_worldMatrix.Mul(m_worldMatrix, transMatrix);
	
	//スケルトンの更新。
	m_skeleton.Update(m_worldMatrix);

	//
	if (m_isShadowCaster) {
		g_graphicsEngine->GetCascadeShadowMap()->RegistShadowCaster(this);
	}
}
void SkinModel::Draw(CMatrix viewMatrix, CMatrix projMatrix, EnRenderMode enRenderMode)
{
	//ボックスのアップデート。引数にワールド行列を渡して更新
	m_box.Update(m_worldMatrix);
	m_culling.Execute(m_box);
	if (m_culling.IsCulling()) {
		return;
	}

	DirectX::CommonStates state(g_graphicsEngine->GetD3DDevice());

	ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	auto shadowMap = g_graphicsEngine->GetShadowMap();
	auto cascadeMap = g_graphicsEngine->GetCascadeShadowMap();

	//定数バッファの内容を更新。
	SVSConstantBuffer vsCb;
	vsCb.mWorld = m_worldMatrix;
	if (enRenderMode == enRenderMode_Normal) {
		vsCb.mProj = projMatrix;
		vsCb.mView = viewMatrix;
		for (int i = 0; i < CascadeShadowMap::SHADOWMAP_NUM; i++) {
			vsCb.mLightViewProj[i] = cascadeMap->GetLightViewProjMatrix(i);
			vsCb.mFarList[i] = { cascadeMap->GetFar(i),0.0f,0.0f,0.0f };
			ID3D11ShaderResourceView* srvArray[]{
				cascadeMap->GetRenderTarget(i)->GetRenderTargetSRV()
			};
			//引数がポインタのポインタ、t2なので引数を2、1にしてる
			d3dDeviceContext->PSSetShaderResources(3 + i, 1, srvArray);
		}
	}
	else if (enRenderMode == enRenderMode_CreateCascadeShadowMap) {
		vsCb.mLightViewProj[0] = cascadeMap->GetLightViewProjMatrix();
	}
	else if (enRenderMode == enRenderMode_CreateShadowMap) {
		vsCb.mView = shadowMap->GetLightViewMatrix();
		vsCb.mProj = shadowMap->GetLightProjMatrix();
	}


	vsCb.mLightView = shadowMap->GetLightViewMatrix();
	vsCb.mLightProj = shadowMap->GetLightProjMatrix();

	if (m_isShadowReciever) {
		vsCb.isShadowReciever = 1;
	}
	else {
		vsCb.isShadowReciever = 0;
	}

	ID3D11ShaderResourceView* srvArray[]{
		shadowMap->GetShadowMapSRV()
	};
	//引数がポインタのポインタ、t2なので引数を2、1にしてる
	d3dDeviceContext->PSSetShaderResources(2, 1, srvArray);
	d3dDeviceContext->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);
	//視点を更新
	m_light.eyePos = g_camera3D.GetPosition();
	//ライト用の定数バッファを更新
	d3dDeviceContext->UpdateSubresource(m_lightCb, 0 ,nullptr,&m_light,0,0);
	//定数バッファをGPUに転送。
	d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
	d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cb);
	d3dDeviceContext->PSSetConstantBuffers(1, 1, &m_lightCb);
	//サンプラステートを設定。
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
	//アルベドテクスチャを設定する。
	d3dDeviceContext->PSSetShaderResources(0, 1, &m_albedoTextureSRV);
	//ボーン行列をGPUに転送。
	m_skeleton.SendBoneMatrixArrayToGPU();

	//エフェクトにクエリを行う
	m_modelDx->UpdateEffects([&](DirectX::IEffect* material) {
		auto modelMaterial = reinterpret_cast<ModelEffect*>(material);
		modelMaterial->SetRenderMode(enRenderMode);
	});

	//描画。
	m_modelDx->Draw(
		d3dDeviceContext,
		state,
		m_worldMatrix,
		viewMatrix,
		projMatrix
	);
}
