#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "Enemy_asigaru.h"
#include "Enemy_Busyo.h"
#include "Enemy_Bomber.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "GameData.h"
#include "UI.h"
#include "gameObject/ysGameObjectManager.h"
#include "InGameSoundDirector.h"
#include "Fade.h"

//�|���ꂽ���̎w�W
//const int knockDownNum = 50;
//�R���X�g���N�^���Ă΂��ƃ��x���ŃL������\��������悤�ɂ��Ă���
Game::Game()
{
	g_goMgr.ResetCount();
	//��SE�f��
	InGameSoundDirector::GetInstans().SoundInit();
	InGameSoundDirector::GetInstans().InGameStartUpBGM();
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
	bool isHoge = true;
	int enemyCount = 0;
	//�G�̑����͉��́H�����܂��B
	g_goMgr.EnemyNumResetCount();
	//���x���Ń��f�����o���B
	m_level.Init(L"Assets/level/musou_honkakustage.tkl",
		[&](const LevelObjectData& objdata) {
			//���y
			enemyCount++;
				if (wcscmp(objdata.name, L"asigaru") == 0) {
					g_goMgr.EnemyCounting();
					//�C���X�^���X�̍쐬
					m_enemy = g_goMgr.NewGameObject<Enemy_asigaru>("Enemy_asigaru");
					m_enemy->SetPos(objdata.position);
					//enemy->SetRot(objdata.rotation);
					m_enemy->SetPlayerInfo(m_player);
					m_enemy->SetGameCameraInfo(m_gameCamera);
					//�ϒ��z��Ɂ��̃C���X�^���X��ǉ�
					return true;
				}
				if (wcscmp(objdata.name, L"enemy_busyo") == 0) {
					//g_goMgr.EnemyCounting();
					//�C���X�^���X�̍쐬
					m_enemy = g_goMgr.NewGameObject<Enemy_Busyo>("Enemy_busyo");
					m_enemy->SetPos(objdata.position);
					m_enemy->SetPlayerInfo(m_player);
					m_enemy->SetGameinfo(this);
					return true;
				}
				//if (enemyCount < 1) {
				if (wcscmp(objdata.name, L"asigaru_taicho") == 0) {
					//�C���X�^���X�̍쐬
					//g_goMgr.EnemyCounting();
					isHoge = false;
					m_enemy = g_goMgr.NewGameObject<Enemy_Bomber>("Enemy_bomber");
					m_enemy->SetPos(objdata.position);
					m_enemy->SetPlayerInfo(m_player);
					m_enemy->SetGameinfo(this);
					return true;
				}
			//}
			return true;
		});
	Fade::Getinstance().StartFadeOut();
	//���C���ƂȂ郌���_�����O�^�[�Q�b�g
	//m_renderTarget.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R16G16B16A16_UNORM);
}

Game::~Game()
{
	g_goMgr.DeleteGOObject(m_player);
	g_goMgr.DeleteGOObject(m_backGround);
	g_goMgr.DeleteGOObject(m_ui);
	g_goMgr.DeleteGOObject(m_gameCamera);
	g_goMgr.DeleteGOObject(m_gamedata);
	//������140��Ă΂�Ă���
	g_goMgr.FindGameObjects<Enemy>("Enemy_asigaru", [&](Enemy* enemy) {
		g_goMgr.DeleteGOObject(enemy);
		//if (enemy != nullptr) {
		//	enemy = nullptr;
		//}
		g_goMgr.EnemyNumSubtract();
		return true;
		});
	g_goMgr.FindGameObjects<Enemy>("Enemy_busyo", [&](Enemy* enemy) {
		g_goMgr.EnemyNumSubtract();
		//if (enemy != nullptr) {
		//	enemy = nullptr;
		//}
		g_goMgr.DeleteGOObject(enemy);
		return true;
		});
	g_goMgr.FindGameObjects<Enemy>("Enemy_bomber", [&](Enemy* enemy) {
		g_goMgr.EnemyNumSubtract();
		//if (enemy != nullptr) {
		//	enemy = nullptr;
		//}
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
