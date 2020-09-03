#include "stdafx.h"
#include "graphics/SkinModelEffect.h"
#include "graphics/SkinModelShaderConst.h"

void __cdecl ModelEffect::Apply(ID3D11DeviceContext* deviceContext)
{
	////法線マップを使用するかのフラグを立てる
	//if (m_normalMapSRV != nullptr) {
	//	vsCb.isHasNormalMap = 1;
	//}
	//else {
	//	vsCb.isHasNormalMap = 0;
	//}
	if (m_normalMapSRV != nullptr) {
		//法線マップが設定されていたらレジストに追加する
		deviceContext->PSSetShaderResources(7, 1, &m_normalMapSRV);
	}
	switch (m_renderMode) {
	case enRenderMode_Normal:
		deviceContext->VSSetShader((ID3D11VertexShader*)m_pVSShader->GetBody(), NULL, 0);
		deviceContext->PSSetShader((ID3D11PixelShader*)m_pPSShader->GetBody(), NULL, 0);
		deviceContext->PSSetShaderResources(enSkinModelSRVReg_AlbedoTexture, 1, &m_albedoTex);
		break;
	case enRenderMode_CreateCascadeShadowMap:
		deviceContext->VSSetShader((ID3D11VertexShader*)m_vsCascadeShadowMap.GetBody(), NULL, 0);
		deviceContext->PSSetShader((ID3D11PixelShader*)m_psShadowMap.GetBody(), NULL, 0);
		break;
	case enRenderMode_CreateShadowMap:
		deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShadowMap.GetBody(), NULL, 0);
		deviceContext->PSSetShader((ID3D11PixelShader*)m_psShadowMap.GetBody(), NULL, 0);
		break;
	}
}