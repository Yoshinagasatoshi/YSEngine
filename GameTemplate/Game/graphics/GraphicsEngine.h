#pragma once
#include "graphics/RenderTarget.h"
#include "graphics/Sprite.h"

/// <summary>
 /// �����_�����O���[�h�B
 /// </summary>
enum EnRenderMode {
	enRenderMode_Invalid,			//�s���ȃ����_�����O���[�h�B
	enRenderMode_CreateCascadeShadowMap,    //�J�X�P�[�h�V���h�E�}�b�v����
	enRenderMode_CreateShadowMap,	//�V���h�E�}�b�v�����B
	enRenderMode_Normal,			//�ʏ탌���_�����O�B
	enRenderMode_Num,				//�����_�����O���[�h�̐��B
};

class ShadowMap;

/*!
 *@brief	�O���t�B�b�N�X�G���W���B
 */
class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();
	/*!
	 *@brief	�������B
	 *@param[in]	hWnd		�E�B���h�E�n���h���B
	 */
	void Init(HWND hWnd);
	/*!
	 *@brief	����B
	 */
	void Release();
	/*!
	 *@brief	D3D11�f�o�C�X���擾�B
	 */
	ID3D11Device* GetD3DDevice()
	{
		return m_pd3dDevice;
	}
	/*!
	 *@brief	D3D11�f�o�C�X�R���e�L�X�g���擾�B
	 */
	ID3D11DeviceContext* GetD3DDeviceContext()
	{
		return m_pd3dDeviceContext;
	}
	ShadowMap* GetShadowMap()
	{
		return m_shadowMap;
	}
	/*!
	 *@brief	�`��J�n�B
	 */
	void BegineRender();
	/*!
	 *@brief	�`��I���B
	 */
	void EndRender();
	void ChangeRenderTarget(ID3D11DeviceContext* d3dDeviceContext, RenderTarget* renderTarget, D3D11_VIEWPORT* viewport);
	void ChangeRenderTarget(ID3D11DeviceContext* d3dDeviceContext, ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStensil, D3D11_VIEWPORT* viewport);
	void Render();
	void ForwardRender();
	void PostRender();
	/// <summary>
	/// �V���h�E�}�b�v�𐶐�
	/// </summary>
	void RenderToShadowMap();
private:
	D3D_FEATURE_LEVEL		m_featureLevel;				//Direct3D �f�o�C�X�̃^�[�Q�b�g�ƂȂ�@�\�Z�b�g�B
	ID3D11Device*			m_pd3dDevice = NULL;		//D3D11�f�o�C�X�B
	IDXGISwapChain*			m_pSwapChain = NULL;		//�X���b�v�`�F�C���B
	ID3D11DeviceContext*	m_pd3dDeviceContext = NULL;	//D3D11�f�o�C�X�R���e�L�X�g�B
	ID3D11RenderTargetView* m_backBuffer = NULL;		//�o�b�N�o�b�t�@�B
	ID3D11RasterizerState*	m_rasterizerState = NULL;	//���X�^���C�U�X�e�[�g�B
	ID3D11Texture2D*		m_depthStencil = NULL;		//�f�v�X�X�e���V���B
	ID3D11DepthStencilView* m_depthStencilView = NULL;	//�f�v�X�X�e���V���r���[�B

	RenderTarget m_renderTarget;				//���C�������_�����O�^�[�Q�b�g
	Sprite m_copyMainRtToFrameBufferSprite;		//���C�������_�����O�ɕ`���ꂽ�G���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g
	D3D11_VIEWPORT m_frameBufferViewports;		//�t���[���o�b�t�@�̃r���[�|�[�g
	ID3D11RenderTargetView* m_frameBufferRenderTargetView = nullptr;	//�t���[���o�b�t�@�̃����_�����O�^�[�Q�b�g�r���[
	ID3D11DepthStencilView* m_frameBufferDepthStencilView = nullptr;	//�t���[���o�b�t�@�̃f�v�X�X�e���V���r���[

	bool m_isWireDraw = false;					//���C���[�t���[����\�����邩�ǂ����Btrue�ł��Ă���
	CD3D11_VIEWPORT m_viewPort;
	ShadowMap* m_shadowMap;
};

extern GraphicsEngine* g_graphicsEngine;			//�O���t�B�b�N�X�G���W��