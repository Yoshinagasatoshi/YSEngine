#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "Enemy_asigaru.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "level/Level.h"
#include "GameData.h"
//#include "UI.h"
#include "gameObject/ysGameObjectManager.h"
//�R���X�g���N�^���Ă΂��ƃ��x���ŃL������\��������悤�ɂ��Ă���
Game::Game()
{
	//�v���C���[
	m_player = g_goMgr.NewGameObject<Player>();
	m_backGround = g_goMgr.NewGameObject<BackGround>();
	m_gameCamera = g_goMgr.NewGameObject<GameCamera>();
	m_gamedata = g_goMgr.NewGameObject<GameData>();
//	m_ui = g_goMgr.NewGameObject<UI>();
	m_gameCamera->SetPlayerInfo(m_player);
	m_gamedata->SetPlayerInfo(m_player);
	Level level;
	level.Init(L"Assets/level/musou_stage.tkl",
		[&](const LevelObjectData& objdata) {
		//�z�u����Ă���I�u�W�F�N�g��asigaru�Ȃ�true
		//wcscmp�͕�����̔�r���s���֐��B
			if (wcscmp(objdata.name, L"asigaru") == 0) {
				//�C���X�^���X�̍쐬
				m_enemy = g_goMgr.NewGameObject<Enemy_asigaru>();
				m_enemy->SetPos(objdata.position);
				//enemy->SetRot(objdata.rotation);
				m_enemy->SetPlayerInfo(m_player);
				//�ϒ��z��Ɂ��̃C���X�^���X��ǉ�
				return true;
			}
		});
	//���C���ƂȂ郌���_�����O�^�[�Q�b�g
	m_renderTarget.Create(1280, 720, DXGI_FORMAT_R16G16B16A16_UNORM);

	//���ɕ`���ꂽ�G��
	//�t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g�̏�����
	//�X�v���C�g�N���X���˂�����[��B���܂�
	//Enemy enemy = new Enemy;
	//m_Player->SetEnemy(enemy);
}

Game::~Game()
{
	g_goMgr.DeleteGOObject(m_player);
	g_goMgr.DeleteGOObject(m_backGround);
}

void Game::Update()
{
	Render();
}

void Game::Draw()
{
	/// <summary>
	/// render�֐��̏�����Draw�֐��ɏ����Ă�������
	/// �I�[�o�[�t���[���N�����̂ňϑ������B
	/// </summary>
}

void Game::Render()
{
	//�����_�����O���[�h�B
	EnRenderMode renderMode = enRenderMode_NORMAL;
	//�`��J�n
	g_graphicsEngine->BegineRender();
	/// <summary>
	/// �I�t�X�N���[�������^�����O
	/// </summary>
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	//���݂̃����_�����O�^�[�Q�b�g���o�b�N�A�b�v���Ă����B
	ID3D11RenderTargetView* oldRenderTargetView;
	ID3D11DepthStencilView* oldDepthStencilView;
	d3dDeviceContext->OMGetRenderTargets(1, &oldRenderTargetView, &oldDepthStencilView);
	//�����_�����O�^�[�Q�b�g��؂�ւ���B
	ID3D11RenderTargetView* rts[] = {
		m_renderTarget.GetRenderTargetView()
	};
	d3dDeviceContext->OMSetRenderTargets(1, rts, m_renderTarget.GetDepthStensilView());
	//�����_�����O�^�[�Q�b�g���N���A�B
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; //red,green,blue,alpha
	m_renderTarget.ClearRenderTarget(clearColor);
	g_goMgr.Draw();
	////�w�i��`��B
	//m_backGround->Draw();
	////�ʏ탌���_�����O�B
	//m_player->Draw();
	//m_enemy->Draw();
	/// <summary>
	/// ��������I���X�N���[�������^�����O
	/// </summary>
	//�����_�����O�^�[�Q�b�g����ɖ߂��B
	d3dDeviceContext->OMSetRenderTargets(1, &oldRenderTargetView, oldDepthStencilView);
	//�����_�����O�^�[�Q�b�g�ƃf�v�X�X�e���V���̎Q�Ƃ�����
	oldRenderTargetView->Release();
	oldDepthStencilView->Release();

	g_goMgr.Draw();
	//m_backGround->Draw();
	//m_player->Draw();
	//m_enemy->Draw();

	g_graphicsEngine->EndRender();
}