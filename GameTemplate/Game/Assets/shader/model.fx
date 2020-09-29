/*!
 * @brief	モデルシェーダー。
 */


/////////////////////////////////////////////////////////////
// Shader Resource View
/////////////////////////////////////////////////////////////
//アルベドテクスチャ。
Texture2D<float4> albedoTexture : register(t0);	
//ボーン行列
StructuredBuffer<float4x4> boneMatrix : register(t1);
Texture2D<float4> shadowMap : register(t2);		//todo シャドウマップ。
Texture2D<float4> cascadeShadowMap1 : register(t3);		//todo カスケードシャドウマップ。
Texture2D<float4> cascadeShadowMap2 : register(t4);		//todo カスケードシャドウマップ。
Texture2D<float4> cascadeShadowMap3 : register(t5);		//todo カスケードシャドウマップ。
Texture2D<float4> cascadeShadowMap4 : register(t6);		//todo カスケードシャドウマップ。
Texture2D<float4> NormalMap : register(t7);		//法線マップ
Texture2D<float4> SpecularMap : register(t8);		//スペキュラマップ

/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler Sampler : register(s0);

/////////////////////////////////////////////////////////////
// 定数バッファ。
/////////////////////////////////////////////////////////////
static const int NUM_CASCADES = 3;
/*!
 * @brief	頂点シェーダーとピクセルシェーダー用の定数バッファ。
 */
cbuffer VSPSCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	//todo ライトビュー行列を追加。
	float4x4 mLightView;	//ライトビュー行列。
	float4x4 mLightProj;	//ライトプロジェクション行列。
	float4x4 mLightViewProj[NUM_CASCADES];    //ライトビュープロジェクション行列
	float4 mFarList[NUM_CASCADES];
	int isShadowReciever;	//シャドウレシーバーフラグ。
	int isHasNormalMap;		//法線マップ、ある？
	int isHasSpecularMap;	//スペキュラマップ、ある？

};
/// <summary>
/// ライトの数
/// </summary>
static const int Light_number = 4;
/*
ディレクションライト
*/
struct SDirectionLight {
	//ディレクションライトのベクトル
	float4 Direction[Light_number];
	//ディレクションライトの色
	float4 Color[Light_number];
};
/// <summary>
/// ライト用の定数バッファ
/// </summary>
cbuffer LightCB : register(b1) {
	SDirectionLight directionLight; //ディレクションライト
	float3          eyePos;         //カメラの視点
	float           specPow;        //スペキュラライトの絞り
	float3          Amblight;
}

/////////////////////////////////////////////////////////////
//各種構造体
/////////////////////////////////////////////////////////////
/*!
 * @brief	スキンなしモデルの頂点構造体。
 */
struct VSInputNmTxVcTangent
{
    float4 Position : SV_Position;			//頂点座標。
    float3 Normal   : NORMAL;				//法線。
    float3 Tangent  : TANGENT;				//接ベクトル。
    float2 TexCoord : TEXCOORD0;			//UV座標。
};
/*!
 * @brief	スキンありモデルの頂点構造体。
 */
struct VSInputNmTxWeights
{
    float4 Position : SV_Position;			//頂点座標。
    float3 Normal   : NORMAL;				//法線。
    float2 TexCoord	: TEXCOORD0;			//UV座標。
    float3 Tangent	: TANGENT;				//接ベクトル。
    uint4  Indices  : BLENDINDICES0;		//この頂点に関連付けされているボーン番号。x,y,z,wの要素に入っている。4ボーンスキニング。
    float4 Weights  : BLENDWEIGHT0;			//この頂点に関連付けされているボーンへのスキンウェイト。x,y,z,wの要素に入っている。4ボーンスキニング。
};

/// <summary>
/// シャドウマップ用のピクセルシェーダへの入力構造体。
/// </summary>
struct PSInput_ShadowMap {
	float4 Position 			: SV_POSITION;	//座標。
};

/*!
 * @brief	ピクセルシェーダーの入力。
 */
struct PSInput{
	float4 Position 	: SV_POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord 	: TEXCOORD0;
	float3 worldPos		: TEXCOORD1;	//ワールド座標。解説６
	float4 posInLVP		: TEXCOORD2;	//ライトビュープロジェクション空間での座標
	float4 posInCamera  : TEXCOORD3;	//ビュー座標
	float4 posInLVP2[NUM_CASCADES]	: TEXCOORD4;
};
/*!
 *@brief	スキン行列を計算。
 */
float4x4 CalcSkinMatrix(VSInputNmTxWeights In)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
        w += In.Weights[i];
    }
    
    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
    return skinning;
}
/// <summary>
/// スペキュラライトを計算する。
/// PSMainで書いていたが、行数が多くなりすぎたので
/// 分けて書くことにしました。
/// </summary>
float3 CalcSpecularLight(float3 normal, float3 worldPos, float2 uv)
{
	float3 lig = 0.0f;
	for (int i = 0; i < Light_number; i++) {
		//① ライトを当てる面から視点に伸びるベクトルtoEyeDirを求める。
		//	 視点の座標は定数バッファで渡されている。LightCbを参照するように。
		float3 toEyeDir = normalize(eyePos - worldPos);
		//② １で求めたtoEyeDirの反射ベクトルを求める。
		float3 reflectEyeDir = -toEyeDir + 2 * dot(normal, toEyeDir) * normal;
		//③ ２で求めた反射ベクトルとディレクションライトの方向との内積を取って、スペキュラの強さを計算する。
		float t = max(0.0f, dot(directionLight.Direction[i], reflectEyeDir));
		//④ pow関数を使って、スペキュラを絞る。絞りの強さは定数バッファで渡されている。
		float specPower = 1.0f;
		if (isHasSpecularMap) {
			//スペキュラマップがある。
			specPower = SpecularMap.Sample(Sampler, uv).r;
		}
		float3 specLig = pow(t, 2.0f) * directionLight.Color[i] * specPower *  7.0f;
		//⑤ スペキュラ反射が求まったら、ligに加算する。
		//鏡面反射を反射光に加算する。
		lig += specLig;
	}
	return lig;
}

/*!--------------------------------------------------------------------------------------
 * @brief	スキンなしモデル用の頂点シェーダー。
-------------------------------------------------------------------------------------- */
PSInput VSMain( VSInputNmTxVcTangent In ) 
{
	PSInput psInput = (PSInput)0;
	float4 pos = mul(mWorld, In.Position);

	//ワールド座標をピクセルシェーダに渡す。
	psInput.worldPos = pos;
	if (isShadowReciever == 1) {
		float4 p = pos;
		p = mul(mLightView, p);
		psInput.posInLVP = mul(mLightProj, p);
		//ライトビュープロジェクション座標に変換
		for (int i = 0; i < NUM_CASCADES; i++) {
			psInput.posInLVP2[i] = mul(mLightViewProj[i], pos);
		}
	}
	pos = mul(mView, pos);
	psInput.posInCamera = pos;
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
	psInput.Normal = normalize(mul(mWorld, In.Normal));
	psInput.Tangent = normalize(mul(mWorld, In.Tangent));
	return psInput;
}

/*!--------------------------------------------------------------------------------------
 * @brief	スキンありモデル用の頂点シェーダー。
 * 全ての頂点に対してこのシェーダーが呼ばれる。
 * Inは1つの頂点データ。VSInputNmTxWeightsを見てみよう。
-------------------------------------------------------------------------------------- */
PSInput VSMainSkin( VSInputNmTxWeights In ) 
{
	PSInput psInput = (PSInput)0;
	
	///////////////////////////////////////////////////
	//ここからスキニングを行っている箇所。
	//スキン行列を計算。
	///////////////////////////////////////////////////
	float4x4 skinning = 0;	
	float4 pos = 0;
	{
	
		float w = 0.0f;
	    for (int i = 0; i < 3; i++)
	    {
			//boneMatrixにボーン行列が設定されていて、
			//In.indicesは頂点に埋め込まれた、関連しているボーンの番号。
			//In.weightsは頂点に埋め込まれた、関連しているボーンのウェイト。
	        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
	        w += In.Weights[i];
	    }
	    //最後のボーンを計算する。
	    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	  	//頂点座標にスキン行列を乗算して、頂点をワールド空間に変換。
		//mulは乗算命令。
	    pos = mul(skinning, In.Position);
		if (isShadowReciever == 1) {
			float4 p = pos;
			p = mul(mLightView, p);
			psInput.posInLVP = mul(mLightProj, p);
			//ライトビュープロジェクション座標に変換
			for (int i = 0; i < NUM_CASCADES; i++) {
				psInput.posInLVP2[i] = mul(mLightViewProj[i], pos);
			}
		}
	}
	
	psInput.Normal = normalize( mul(skinning, In.Normal) );
	psInput.Tangent = normalize( mul(skinning, In.Tangent) );
	
	//ワールド座標をピクセルシェーダに渡す。
	psInput.worldPos = pos;

	pos = mul(mView, pos);
	psInput.posInCamera = pos;
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
    return psInput;
}
//--------------------------------------------------------------------------------------
// ピクセルシェーダーのエントリ関数。
//--------------------------------------------------------------------------------------
float4 PSMain(PSInput In) : SV_Target0
{
	//法線マップ
float3 normal = 0;
//通ってない…通ってたらめちゃ光る。
if (isHasNormalMap == 1) {
	//法線マップがある時の処理。
	//法線と説ベクトルの外積を計算して、従ベクトルを計算する。
	float3 biNormal = cross(In.Normal, In.Tangent);
	float3 tangentSpaceNormal = NormalMap.Sample(Sampler, In.TexCoord);
	//0.0 ~ 1.0の範囲になっているタンジェントスペース法線を
	//-1.0 ~ 1.0 の範囲に変更
	tangentSpaceNormal = (tangentSpaceNormal * 2.0f) - 1.0f;
	//法線をタンジェントスペースから、ワールドスペースに変換する
	normal = In.Tangent * tangentSpaceNormal.x + biNormal * tangentSpaceNormal.y + In.Normal * tangentSpaceNormal.z;
}
else {
	//ない時の処理。。
	normal = In.Normal;
}

	/// <summary>
	/// ライトのコード
	/// </summary>
	//albedoテクスチャからカラーをフェッチする。
	//拡散反射
	float4 albedoColor = albedoTexture.Sample(Sampler, In.TexCoord);
	float3 lig = 0.0f;
    for (int i = 0; i < Light_number; i++) {
		float t = max(0.0f, dot(normal * -1.0f, directionLight.Direction[i]) * directionLight.Color[i]);
		if (t < 0.2f) {
			lig += directionLight.Color[i] * 0.5f;
		}
		else {
			lig += directionLight.Color[i]*1.5f;
		}
	}

	lig += CalcSpecularLight(normal, In.worldPos, In.TexCoord);

	//4　環境光を当てる。
	//lig += Amblight;

	if (isShadowReciever == 1) {
		/*//LVP空間から見た時の最も手前の深度値をシャドウマップから取得する。
		//プロジェクション行列をシャドウマップのUV座標に変換している
		float2 shadowMapUV = In.posInLVP.xy / In.posInLVP.w;
		shadowMapUV *= float2(0.5f, -0.5f);
		shadowMapUV += 0.5f;
		//spsOut.shadow = shadowMapUV.x;
		//シャドウマップのUV座標範囲内かどうかを判定する。
		if (shadowMapUV.x < 1.0f
			&& shadowMapUV.x > 0.0f
			&& shadowMapUV.y < 1.0f
			&& shadowMapUV.y > 0.0f
			) {
			///LVP空間での深度値を計算。
			float zInLVP = In.posInLVP.z / In.posInLVP.w;
			float zInShadowMap = 1.0f;
			//シャドウマップに書き込まれている深度値を取得。
			zInShadowMap = shadowMap.Sample(Sampler, shadowMapUV);
			if (zInLVP > zInShadowMap + 0.0001f) {
				//影が落ちているので、光を弱くする
				lig *= 0.5f;
			}
		}*/
		for (int i = 0; i < NUM_CASCADES; i++) {
			if (mFarList[i].x > In.posInCamera.z) {
				//LVP空間から見た時の最も手前の深度値をシャドウマップから取得する。
				//プロジェクション行列をシャドウマップのUV座標に変換している
				float2 shadowMapUV = In.posInLVP2[i].xy / In.posInLVP2[i].w;
				shadowMapUV *= float2(0.5f, -0.5f);
				shadowMapUV += 0.5f;
				//spsOut.shadow = shadowMapUV.x;
				//シャドウマップのUV座標範囲内かどうかを判定する。
				if (shadowMapUV.x <= 1.0f
					&& shadowMapUV.x >= 0.0f
					&& shadowMapUV.y <= 1.0f
					&& shadowMapUV.y >= 0.0f
					) {
					///LVP空間での深度値を計算。
					float zInLVP = In.posInLVP2[i].z / In.posInLVP2[i].w;
					float zInShadowMap = 1.0f;
					//シャドウマップに書き込まれている深度値を取得。
					if (i == 0) {
						zInShadowMap = cascadeShadowMap1.Sample(Sampler, shadowMapUV);
					}
					else if (i == 1) {
						zInShadowMap = cascadeShadowMap2.Sample(Sampler, shadowMapUV);
					}
					else if (i == 2) {
						zInShadowMap = cascadeShadowMap3.Sample(Sampler, shadowMapUV);
					}
					else if (i == 3) {
						zInShadowMap = cascadeShadowMap4.Sample(Sampler, shadowMapUV);
					}
					if (zInLVP > zInShadowMap + 0.0001f * 2.0f * (4 - i)) {
						//影が落ちているので、光を弱くする
						lig *= 0.5f;

					}
					break;


				}
			}
		}
	}



	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	finalColor.xyz = albedoColor.xyz * lig;

	return finalColor;
	//return albedoTexture.Sample(Sampler, In.TexCoord);
}
//-----------------------------------------------
// シルエット描画用のピクセルシェーダーのエントリー関数
//-----------------------------------------------


// <summary>
/// スキン無しシャドウマップ生成用の頂点シェーダー。
/// </summary>
PSInput_ShadowMap VSMain_ShadowMap(VSInputNmTxVcTangent In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	//ライトプロジェクション行列に変換している
	float4 pos = mul(mWorld, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	//pos = mul(mLightView, pos);
	//pos = mul(mLightProj, pos);
	psInput.Position = pos;
	return psInput;
}

// <summary>
/// スキンありシャドウマップ生成用の頂点シェーダー。
/// </summary>
PSInput_ShadowMap VSMainSkin_ShadowMap(VSInputNmTxWeights In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	float4x4 skinning = CalcSkinMatrix(In);
	float4 pos = mul(skinning, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	//pos = mul(mLightView, pos);
	//pos = mul(mLightProj, pos);
	psInput.Position = pos;
	return psInput;
}

// <summary>
/// スキン無しカスケードシャドウマップ生成用の頂点シェーダー。
/// </summary>
PSInput_ShadowMap VSMain_CascadeShadowMap(VSInputNmTxVcTangent In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	//ライトプロジェクション行列に変換している
	float4 pos = mul(mWorld, In.Position);
	pos = mul(mLightViewProj[0], pos);
	psInput.Position = pos;
	return psInput;
}

// <summary>
/// スキンありカスケードシャドウマップ生成用の頂点シェーダー。
/// </summary>
PSInput_ShadowMap VSMainSkin_CascadeShadowMap(VSInputNmTxWeights In)
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	float4x4 skinning = CalcSkinMatrix(In);
	float4 pos = mul(skinning, In.Position);
	pos = mul(mLightViewProj[0], pos);
	psInput.Position = pos;
	return psInput;
}

/// <summary>
/// ピクセルシェーダーのエントリ関数。
/// </summary>
float4 PSMain_ShadowMap(PSInput_ShadowMap In) : SV_Target0
{
	//射影空間でのZ値を返す。
	return In.Position.z / In.Position.w;
}

