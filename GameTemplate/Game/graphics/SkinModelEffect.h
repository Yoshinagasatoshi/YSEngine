#pragma once

#include "graphics/Shader.h"
#include <array> 

/*!
*@brief	モデルエフェクト。
*/
class ModelEffect : public DirectX::IEffect {
protected:
	std::wstring m_materialName;	//!<マテリアル名。
	Shader* m_pVSShader = nullptr;
	Shader* m_pPSShader = nullptr;
	Shader m_vsShader;
	Shader m_psShader;
	Shader m_psSilhouette;
	bool isSkining;
	ID3D11ShaderResourceView* m_albedoTex = nullptr;
	std::array<ID3D11ShaderResourceView*, 4> m_albedoTextureStack = { nullptr };
	int m_albedoTextureStackPos = 0;
	//レンダーモードは今はつけない
	EnRenderMode m_renderMode = enRenderMode_Invalid;	//レンダリングモード。
	ID3D11DepthStencilState* m_silhouetteDepthStensilState = nullptr;

	Shader m_vsShadowMap;
	Shader m_psShadowMap;

	
	ID3D11ShaderResourceView* m_normalMapSRV = nullptr;		//法線マップのSRV
	ID3D11ShaderResourceView* m_specularMapSRV = nullptr;		//スぺキュラマップのSRV

	Shader m_vsCascadeShadowMap;			//カスケードシャドウマップ生成用の頂点シェーダー

public:
	//改造予定.DirectX3_5
	ModelEffect()
	{
		m_psShader.Load("Assets/shader/model.fx", "PSMain", Shader::EnType::PS);
		m_psShadowMap.Load("Assets/shader/model.fx", "PSMain_ShadowMap", Shader::EnType::PS);
		m_pPSShader = &m_psShader;
	}
	virtual ~ModelEffect()
	{
		if (m_albedoTex) {
			m_albedoTex->Release();
		}
	}
	void __cdecl Apply(ID3D11DeviceContext* deviceContext) override;

	void __cdecl GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength) override
	{
		*pShaderByteCode = m_vsShader.GetByteCode();
		*pByteCodeLength = m_vsShader.GetByteCodeSize();
	}
	void SetAlbedoTexture(ID3D11ShaderResourceView* tex)
	{
		m_albedoTex = tex;
	}
	/// <summary>
	/// 現在のアルベドテクスチャをスタックに避難させる。
	/// </summary>
	void PushAlbedoTexture()
	{
		m_albedoTextureStack[m_albedoTextureStackPos] = m_albedoTex;
		m_albedoTex = nullptr;
		m_albedoTextureStackPos++;
	}
	void PopAlbedoTexture()
	{
		m_albedoTextureStackPos--;
		SetAlbedoTexture(m_albedoTextureStack[m_albedoTextureStackPos]);
	}
	void SetMatrialName(const wchar_t* matName)
	{
		m_materialName = matName;
	}
	
	bool EqualMaterialName(const wchar_t* name) const
	{
		return wcscmp(name, m_materialName.c_str()) == 0;
	}
	void SetRenderMode(EnRenderMode renderMode)
	{
		m_renderMode = renderMode;
	}
	/// <summary>
/// 法線マップを設定する
/// </summary>
	void SetNormalMap(ID3D11ShaderResourceView* srv)
	{
		m_normalMapSRV = srv;
	}
	/// <summary>
	/// スペキュラマップを設定する
	/// </summary>
	void SetSpecularMap(ID3D11ShaderResourceView* srv)
	{
		m_specularMapSRV = srv;
	}
};
/*!
*@brief
*  スキンなしモデルエフェクト。
*/
class NonSkinModelEffect : public ModelEffect {
public:
	NonSkinModelEffect()
	{
		m_vsShader.Load("Assets/shader/model.fx", "VSMain", Shader::EnType::VS);
		m_pVSShader = &m_vsShader;
		isSkining = false;

		//スキン無しシャドウマップ
		m_vsShadowMap.Load("Assets/shader/model.fx", "VSMain_ShadowMap", Shader::EnType::VS);
		m_vsCascadeShadowMap.Load("Assets/shader/model.fx", "VSMain_CascadeShadowMap", Shader::EnType::VS);
	}
};
/*!
*@brief
*  スキンモデルエフェクト。
*/
class SkinModelEffect : public ModelEffect {
public:
	SkinModelEffect()
	{
		wchar_t hoge[256];
		GetCurrentDirectoryW(256, hoge);
		m_vsShader.Load("Assets/shader/model.fx", "VSMainSkin", Shader::EnType::VS);
		
		m_pVSShader = &m_vsShader;
		isSkining = true;

		//スキンありシャドウマップ
		m_vsShadowMap.Load("Assets/shader/model.fx", "VSMainSkin_ShadowMap", Shader::EnType::VS);
		m_vsCascadeShadowMap.Load("Assets/shader/model.fx", "VSMainSkin_CascadeShadowMap", Shader::EnType::VS);
	}
};

/*!
*@brief
*  エフェクトファクトリ。
*/
class SkinModelEffectFactory : public DirectX::EffectFactory {
public:
	SkinModelEffectFactory(ID3D11Device* device) :
		EffectFactory(device) {}
	std::shared_ptr<DirectX::IEffect> __cdecl CreateEffect(const EffectInfo& info, ID3D11DeviceContext* deviceContext)override
	{
		std::shared_ptr<ModelEffect> effect;
		if (info.enableSkinning) {
			//スキニングあり。
			effect = std::make_shared<SkinModelEffect>();
		}
		else {
			//スキニングなし。
			effect = std::make_shared<NonSkinModelEffect>();
		}
		effect->SetMatrialName(info.name);
		if (info.diffuseTexture && *info.diffuseTexture)
		{
			ID3D11ShaderResourceView* texSRV;
			DirectX::EffectFactory::CreateTexture(info.diffuseTexture, deviceContext, &texSRV);
			effect->SetAlbedoTexture(texSRV);
		}
		return effect;
	}

	void __cdecl CreateTexture(const wchar_t* name, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView** textureView) override
	{
		return DirectX::EffectFactory::CreateTexture(name, deviceContext, textureView);
	}
};
