#pragma once
#include "graphics/RenderTarget.h"


class SkinModel;
class CascadeShadowMap


{
public:
	CascadeShadowMap();
	/// <summary>
	/// ライトビュー行列を取得
	/// </summary>
	/// <returns></returns>
	CMatrix GetLightViewMatrix()
	{
		return m_lightViewMatrix;
	}
	/// <summary>
	/// ライトプロジェクション行列を取得
	/// </summary>
	/// <returns></returns>
	CMatrix GetLightProjMatrix()
	{
		return m_lightProjMatrix;
	}
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// ライトビュー行列やライトプロジェクション行列を更新
	/// </summary>
	/// <param name="lightCameraPos"></param>
	/// <param name="lightCameraTarget"></param>
	void UpdateFromLightTaraget(const CVector3& lightCameraPos, const CVector3& lightCameraTarget);
	/// <summary>
	/// 更新(ライトカメラの向きを指定するバージョン)
	/// </summary>
	/// <param name="lightCameraPos"></param>
	/// <param name="lightDir"></param>
	void UpdateFromLightDirection(const CVector3& lightCameraPos, const CVector3& lightDir);
	/// <summary>
	/// シャドウマップにシャドウキャスターをレンダリング
	/// </summary>
	void RenderToShadowMap();
	/// <summary>
	/// シャドウキャスターを追加
	/// </summary>
	/// <param name="shadowCaster"></param>
	void RegistShadowCaster(SkinModel* shadowCaster)
	{
		m_shadowCaters.push_back(shadowCaster);
	}
	/// <summary>
	/// シャドウキャスターを排除
	/// </summary>
	void popShadowCaster()
	{
		m_shadowCaters.pop_back();
	}

	/// <summary>
	/// レンダリングターゲットを取得
	/// </summary>
	/// <returns></returns>
	RenderTarget* GetRenderTarget(int number)
	{
		return &m_shadowMapRT[number];
	}
	/// <summary>
	/// ライトの高さ
	/// </summary>
	/// <param name="lightHeight"></param>
	void SetLightHeight(const float lightHeight)
	{
		m_lightHeight = lightHeight;
	}
	/// <summary>
	/// ライトの方向を設定
	/// </summary>
	/// <param name="lightDir"></param>
	void SetLightDirection(const CVector3& lightDir)
	{
		m_lightDir = lightDir;
	}
	/// <summary>
	/// ライトビュープロジェクション行列を取得
	/// </summary>
	/// <returns></returns>
	CMatrix& GetLightViewProjMatrix()
	{
		return m_lightVieProjMatrix[m_shadowMapNumber];
	}
	CMatrix& GetLightViewProjMatrix(int number)
	{
		return m_lightVieProjMatrix[number];
	}
	/// <summary>
	/// シャドウマップの番号を取得、0からよ
	/// </summary>
	/// <returns></returns>
	int GetShadowMapNumber() const
	{
		return m_shadowMapNumber;
	}
	/// <summary>
	/// ライトの方向を取得
	/// </summary>
	/// <returns></returns>
	CVector3& GetLightDir()
	{
		return m_lightDir;
	}
	/// <summary>
	/// 各シャドウマップの深度値を取得
	/// </summary>
	/// <param name="number"></param>
	/// <returns></returns>
	float GetFar(const int number)
	{
		return m_farList[number];
	}
	/// <summary>
	/// 各シャドウマップのライトビューの逆行列を取得
	/// </summary>
	/// <param name="number"></param>
	/// <returns></returns>
	CMatrix& GetLightViewInv(const int number)
	{
		return m_lightViewMatrixInv[number];
	}
	static const int SHADOWMAP_NUM = 3;
private:
	CVector3 m_lightCameraPosition = CVector3(300.0f, 300.0f, -300.0f);		//ライトカメラの座標	
	CVector3 m_lightCamerataraget = CVector3(0.0f, 0.0f, 0.0f);		//ライトカメラの注視点
	CMatrix m_lightViewMatrix;			//ライトビュー行列
	CMatrix m_lightProjMatrix;			//ライトプロジェクション行列
	RenderTarget m_shadowMapRT[SHADOWMAP_NUM];			//シャドウマップのレンダリングターゲット
	CMatrix m_lightVieProjMatrix[SHADOWMAP_NUM];			//各シャドウマップのビュープロジェクション行列
	std::vector<SkinModel*> m_shadowCaters;		//シャドウキャスターのリスト
	CVector3 m_lightDir = CVector3(0.6396f,-0.6396f,0.4264f);
	float m_lightHeight = 10000.0f;
	int m_shadowMapNumber = 0;
	float m_farList[SHADOWMAP_NUM] = {1.0f,1.0f,1.0f/*,1.0f*/};
	CMatrix m_lightViewMatrixInv[SHADOWMAP_NUM];
	int siken = 0;
};


