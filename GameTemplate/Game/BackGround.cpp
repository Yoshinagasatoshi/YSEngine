#include "stdafx.h"
#include "BackGround.h"
#include "gameObject/ysGameObjectManager.h"

BackGround::BackGround()
{
	//cmoファイルの読み込み。
	m_model.Init(L"Assets/modelData/sotowakutukiStage.cmo");
	//m_model.Init(L"Assets/modelData/musou_stage.cmo");

	//法線マップとスペキュラをロード。
	//normalmap
	DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(),
		L"Assets/modelData/Earth_Normals.dds",		//ロードするテクスチャのパス
		0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
		0,
		0,
		false,
		nullptr,
		&m_earthNormalMapSRV	//作成されたSRVのアドレスの格納先
	);

	DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(),
		L"Assets/modelData/Grass_Normals.dds",		//ロードするテクスチャのパス
		0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
		0,
		0,
		false,
		nullptr,
		&m_grassNormalMapSRV	//作成されたSRVのアドレスの格納先
	);

	m_model.FindMaterial([&](auto material) {
		if (material->EqualMaterialName(L"TUTHI")) {
			material->SetNormalMap(m_earthNormalMapSRV);
		}
		else if (material->EqualMaterialName(L"KUSA")) {
			material->SetNormalMap(m_grassNormalMapSRV);
		}
	});
	m_model.SetNormalMap(m_earthNormalMapSRV);

	//m_model.SetShadowCaster(false);
	//m_physicsStaticObject.CreateCommon(m_position, m_rotation);
	m_physicsStaticObject.CreateMeshObject(m_model, m_position, m_rotation);
}
BackGround::~BackGround()
{
	g_goMgr.DeleteGOObject(this);
}
void BackGround::Update()
{
	//ワールド行列の更新。
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	
}
void BackGround::Draw()
{
	m_model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}