#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "Enemy_asigaru.h"
#include "Enemy_Busyo.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "GameData.h"
#include "UI.h"
#include "gameObject/ysGameObjectManager.h"
#include "Fade.h"

//�|���ꂽ���̎w�W
const int knockDownNum = 50;
//�R���X�g���N�^���Ă΂��ƃ��x���ŃL������\��������悤�ɂ��Ă���
Game::Game()
{
	//InitCamera();
	//�v���C���[
	m_player = g_goMgr.NewGameObject<Player>("Player");
	m_player->SetGameInfo(this);
	m_ui = g_goMgr.NewGameObject<UI>("UI");
	m_ui->SetPlayerInfo(m_player);
	m_backGround = g_goMgr.NewGameObject<BackGround>("BackGround");
	m_gameCamera = g_goMgr.NewGameObject<GameCamera>("GameCamera");
	m_gameCamera->SetPlayerInfo(m_player);
	m_gamedata = g_goMgr.NewGameObject<GameData>("GameData");
	m_gamedata->SetPlayerInfo(m_player);
	//���x���Ń��f�����o���B
	m_level.Init(L"Assets/level/a.tkl",
		//musou_honkakustage.tkl",
		[&](const LevelObjectData& objdata) {
			//���y
			if (wcscmp(objdata.name, L"asigaru") == 0) {
				//�C���X�^���X�̍쐬
				m_enemy = g_goMgr.NewGameObject<Enemy_asigaru>("Enemy_asigaru");
				m_enemy->SetPos(objdata.position);
				//enemy->SetRot(objdata.rotation);
				m_enemy->SetPlayerInfo(m_player);
				m_enemy->SetGameCameraInfo(m_gameCamera);
				//�ϒ��z��Ɂ��̃C���X�^���X��ǉ�
				return true;
			}
			//if (wcscmp(objdata.name, L"enemy_busyo") == 0) {
			//	//�C���X�^���X�̍쐬
			//	m_enemy = g_goMgr.NewGameObject<Enemy_Busyo>("Enemy_busyo");
			//	m_enemy->SetPos(objdata.position);
			//	m_enemy->SetPlayerInfo(m_player);
			//	return true;
			//}
		});
	Fade::Getinstance().StartFadeOut();
	////���C���ƂȂ郌���_�����O�^�[�Q�b�g
	//m_renderTarget.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R16G16B16A16_UNORM);

	////���ɕ`���ꂽ�G��
	////�t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g�̏���������
	//m_copyMainRtToFrameBufferSprite.Init(
	//	m_renderTarget.GetRenderTargetSRV(),
	//	FRAME_BUFFER_W,
	//	FRAME_BUFFER_H
	//);
}

Game::~Game()
{
	g_goMgr.DeleteGOObject(m_player);
	g_goMgr.DeleteGOObject(m_backGround);
	g_goMgr.DeleteGOObject(m_ui);
	g_goMgr.DeleteGOObject(m_gameCamera);
	g_goMgr.DeleteGOObject(m_gamedata);
	g_goMgr.FindGameObjects<Enemy>("Enemy_asigaru", [&](Enemy* enemy) {
		g_goMgr.DeleteGOObject(enemy);
		return true;
		});
	g_goMgr.FindGameObjects<Enemy>("Enemy_busyo", [&](Enemy* enemy) {
		g_goMgr.DeleteGOObject(enemy);
		return true;
		});
	g_goMgr.ResetCount();
}

void Game::Update()
{
	if (m_gameDelete) {
		m_gameDelete = false;
		g_goMgr.DeleteGOObject(this);
	}
	if (g_goMgr.GetCount() > knockDownNum) {
		if (!m_busyofrag) {
			m_busyofrag = true;
			//���x���Ń��f�����o���B
			m_level_EnemyBusyo.Init(L"Assets/level/musou_EnemyBusyoLevel.tkl",
			[&](const LevelObjectData& objdata) {
				if (wcscmp(objdata.name, L"enemy_busyo") == 0) {
					//�C���X�^���X�̍쐬
					m_enemy = g_goMgr.NewGameObject<Enemy_Busyo>("Enemy_busyo");
					m_enemy->SetPos(objdata.position);
					m_enemy->SetPlayerInfo(m_player);
					m_enemy->SetGameinfo(this);
					return true;
				}
			});
		}
	}

}


void Game::Draw()
{
	/// <summary>
	/// render�֐��̏�����Draw�֐��ɏ����Ă�������
	/// �I�[�o�[�t���[���N�����̂�Render�Ɉϑ������B
	/// </summary>
}


void Game::GameDelete()
{
	m_gameDelete = true;
}
