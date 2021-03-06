#pragma once

#include "SkinModelEffect.h"
#include "Skeleton.h"
#include "shadow\CascadeShadowMap.h"
#include "../culling/ysObjectFrustumCulling.h"
#include "../culling/ysBox.h"
/*!
*@brief	FBXの上方向。
*/
enum EnFbxUpAxis {
	enFbxUpAxisY,		//Y-up
	enFbxUpAxisZ,		//Z-up
};
/*!
*@brief	スキンモデルクラス。
*/
class CObjectFrustumCulling;
class CBox;
class SkinModel
{
public:
	//メッシュが見つかったときのコールバック関数。
	using OnFindMesh = std::function<void(const std::unique_ptr<DirectX::ModelMeshPart>&)>;
	/*
	*@brief コンストラクタ。
	*/
	SkinModel();
	/*!
	*@brief	デストラクタ。
	*/
	~SkinModel();
	/// <summary>
	/// アップデート
	/// </summary>
	void Update();
	/*!
	*@brief	初期化。
	*@param[in]	filePath		ロードするcmoファイルのファイルパス。
	*@param[in] enFbxUpAxis		fbxの上軸。デフォルトはenFbxUpAxisZ。
	*/
	void Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis = enFbxUpAxisZ);
	/*!
	*@brief	モデルをワールド座標系に変換するためのワールド行列を更新する。
	*@param[in]	position	モデルの座標。
	*@param[in]	rotation	モデルの回転。
	*@param[in]	scale		モデルの拡大率。
	*/
	void UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale);
	/*!
	*@brief	ボーンを検索。
	*@param[in]		boneName	ボーンの名前。
	*@return	見つかったボーン。見つからなかった場合はnullptrを返します。
	*/
	Bone* FindBone(const wchar_t* boneName)
	{
		int boneId = m_skeleton.FindBoneID(boneName);
		return m_skeleton.GetBone(boneId);
	}
	/*!
	*@brief	モデルを描画。
	*@param[in]	viewMatrix		カメラ行列。
	*  ワールド座標系の3Dモデルをカメラ座標系に変換する行列です。
	*@param[in]	projMatrix		プロジェクション行列。
	*  カメラ座標系の3Dモデルをスクリーン座標系に変換する行列です。
	*/
	void Draw( CMatrix viewMatrix, CMatrix projMatrix ,EnRenderMode enRenderMode = enRenderMode_Normal);

	/*!
	*@brief	モデルマテリアルの検索。
	*@param[in]	findEffect		マテリアルを見つけた時に呼ばれるコールバック関数
	*/
	void FindMaterial(std::function<void(SkinModelEffect*)> findMaterial) const
	{
		FindMesh([&](auto& mesh) {
			SkinModelEffect* effect = reinterpret_cast<SkinModelEffect*>(mesh->effect.get());
			findMaterial(effect);
			});
	}
	/// <summary>
	// マテリアルに対してクエリを行う
	/// </summary>
	void QueryMaterials(std::function<void(SkinModelEffect*)> func)
	{
		m_modelDx->UpdateEffects([&](DirectX::IEffect* material) {
			func(reinterpret_cast<SkinModelEffect*>(material));
			});
	}
	/*!
	*@brief	スケルトンの取得。
	*/
	Skeleton& GetSkeleton()
	{
		return m_skeleton;
	}
	/*!
	*@brief	メッシュを検索する。
	*@param[in] onFindMesh		メッシュが見つかったときのコールバック関数
	*/
	void FindMesh(OnFindMesh onFindMesh) const
	{
		for (auto& modelMeshs : m_modelDx->meshes) {
			for (std::unique_ptr<DirectX::ModelMeshPart>& mesh : modelMeshs->meshParts) {
				onFindMesh(mesh);
			}
		}
	}
	CMatrix GetWorldMatrix() const
	{
		return m_worldMatrix;
	}
	/// <summary>
	/// シャドウレシーバーを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetShadowReciever(bool flag)
	{
		m_isShadowReciever = flag;
	}
	/// <summary>
	/// シャドウレシーバーを取得
	/// </summary>
	/// <returns></returns>
	bool GetShadowReciever()
	{
		return m_isShadowReciever;
	}
	/// <summary>
	/// シャドウキャスターを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetShadowCaster(bool flag)
	{
		m_isShadowCaster = flag;
	}
	/// <summary>
	/// シャドウキャスターを取得
	/// </summary>
	/// <returns></returns>
	bool GetShadowCaster()
	{
		return m_isShadowCaster;
	}
	/*!
	*@brief	SRVのレジスタ番号。
	*/
	enum EnSkinModelSRVReg {
		enSkinModelSRVReg_DiffuseTexture = 0,		//!<ディフューズテクスチャ。
		enSkinModelSRVReg_BoneMatrix,				//!<ボーン行列。
	};
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
private:
	/*!
	*@brief	サンプラステートの初期化。
	*/
	void InitSamplerState();
	/*!
	*@brief	定数バッファの作成。
	*/
	void InitConstantBuffer();
	/// <summary>
	/// アルベドテクスチャの初期化。
	/// </summary>
	void InitAlbedoTexture();

	/// <summary>
	/// ディレクションライトの初期化。
	/// </summary>
	void InitDirectionLight();

	/*!
	*@brief	スケルトンの初期化。
	*@param[in]	filePath		ロードするcmoファイルのファイルパス。
	*/
	void InitSkeleton(const wchar_t* filePath);


private:
	//定数バッファ。
	struct SVSConstantBuffer {
		CMatrix mWorld;
		CMatrix mView;
		CMatrix mProj;
		CMatrix mLightView;		//todo ライトビュー行列。
		CMatrix mLightProj;		//todo ライトプロジェクション行列。
		CMatrix mLightViewProj[CascadeShadowMap::SHADOWMAP_NUM];	//ライトビュープロジェクション行列
		CVector4 mFarList[CascadeShadowMap::SHADOWMAP_NUM];
		int isShadowReciever;	//todo シャドウレシーバーのフラグ。
		int isHasNormalMap;		//法線マップを保持しているのか？
		//CMatrix mLightViewProj[CascadeShadowMap::SHADOWMAP_NUM];	//ライトビュープロジェクション行列
		//CMatrix mLightViewInv[CascadeShadowMap::SHADOWMAP_NUM];		//ライトビューの逆行列
		//CVector4 mFar[CascadeShadowMap::SHADOWMAP_NUM];
	};
	//ディレクションライトの定数バッファ
	struct SDirectionLight {
		CVector4 direction[4];
		CVector4 color[4];
	};
	//ライトの定数バッファ
	struct SLight {
		SDirectionLight     directionLight; //ディレクションライト
		CVector3            eyePos;         //視点の座標
		float               specPow;        //鏡面販社の絞り
		CVector3            AmbLight;       //アンビエントライト
	};
	EnFbxUpAxis			m_enFbxUpAxis = enFbxUpAxisZ;	   //!<FBXの上方向。
	ID3D11Buffer*		m_cb = nullptr;					   //!<定数バッファ。
	ID3D11Buffer*       m_lightCb = nullptr;               //!<ライト用定数バッファ。
	//SDirectionLight     m_dirLight;                        //!<ディレクションライト
	SLight              m_light;                           //ライトの構造体
	Skeleton			m_skeleton;						   //!<スケルトン。
	CMatrix				m_worldMatrix;					   //!<ワールド行列。
	DirectX::Model*		m_modelDx;					   	   //!<DirectXTKが提供するモデルクラス。
	ID3D11SamplerState* m_samplerState = nullptr;		   //!<サンプラステート。
	ID3D11ShaderResourceView* m_albedoTextureSRV = nullptr;//!<アルベドテクスチャ。
	ID3D11ShaderResourceView* m_normalMapSRV = nullptr;		//法線マップのSRV
	ID3D11ShaderResourceView* m_specularMapSRV = nullptr;		//スぺキュラマップのSRV

	CObjectFrustumCulling m_culling;		//化リング
	CBox m_box;							//化リングに必要なボックスデータ

	const int Lightnumber = 4;
	bool m_isShadowReciever = true;
	bool m_isShadowCaster = true;
};

