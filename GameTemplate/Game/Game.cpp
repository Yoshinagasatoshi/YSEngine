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
//���S�Ɉ�v�ł��邩�H
const int String_match = 0;	//������ɈႢ�͂Ȃ������m�F���邽�߂Ɏg�p����ϐ�

//�|���ꂽ���̎w�W
//const int knockDownNum = 50;
//�R���X�g���N�^���Ă΂��ƃ��x���ŃL������\��������悤�ɂ��Ă���
Game::Game()
{
	//�I�u�W�F�N�g�̃J�E���g�����Z�b�g
	g_goMgr.ResetCount();
	//���o�Q�[�W�̃��Z�b�g
	g_goMgr.ResetMusouGauge();
	//��SE�f��
	InGameSoundDirector::GetInstans().SoundInit();
	//InGameSoundDirector::GetInstans().InGameStartUpBGM();
	//InitCamera();
	//�v���C���[�Ȃǂ̃C���X�^���X���쐬
	m_player = g_goMgr.NewGameObject<Player>("Player");
	m_player->SetGameInfo(this);
	m_ui = g_goMgr.NewGameObject<UI>("UI");
	m_ui->SetPlayerInfo(m_player);
	m_ui->SetGameInfo(this);
	m_backGround = g_goMgr.NewGameObject<BackGround>("BackGround");
	m_gameCamera = g_goMgr.NewGameObject<GameCamera>("GameCamera");
	m_gameCamera->SetPlayerInfo(m_player);
	m_gamedata = g_goMgr.NewGameObject<GameData>("GameData");
	m_gamedata->SetPlayerInfo(m_player);
	int enemyCount = 0;//�f�o�b�N�p�R�[�h�@��ŏ����܂�
	//�G�̑����͉��́H�����܂��B
	g_goMgr.EnemyNumResetCount();
	//���x���Ń��f�����o���B
	m_level.Init(L"Assets/level/musou_honkakustage.tkl",
		[&](const LevelObjectData& objdata) {
			//���y
				if (wcscmp(objdata.name, L"asigaru") == String_match) {
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
				//�G����
				if (wcscmp(objdata.name, L"enemy_busyo") == String_match) {
					//g_goMgr.EnemyCounting();
					//�C���X�^���X�̍쐬
					m_enemy = g_goMgr.NewGameObject<Enemy_Busyo>("Enemy_busyo");
					m_enemy->SetPos(objdata.position);
					m_enemy->SetPlayerInfo(m_player);
					m_enemy->SetGameinfo(this);
					return true;
				}
				//������(������)
				if (wcscmp(objdata.name, L"asigaru_taicho") == String_match) {
					//�C���X�^���X�̍쐬
					//g_goMgr.EnemyCounting();
					m_enemy = g_goMgr.NewGameObject<Enemy_Bomber>("Enemy_bomber");
					m_enemy->SetPos(objdata.position);
					m_enemy->SetPlayerInfo(m_player);
					m_enemy->SetGameinfo(this);
					return true;
				}
			return true;
		});
	Fade::Getinstance().StartFadeOut();
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

		g_goMgr.EnemyNumSubtract();
		return true;
		});
	g_goMgr.FindGameObjects<Enemy>("Enemy_busyo", [&](Enemy* enemy) {
		g_goMgr.EnemyNumSubtract();

		g_goMgr.DeleteGOObject(enemy);
		return true;
		});
	g_goMgr.FindGameObjects<Enemy>("Enemy_bomber", [&](Enemy* enemy) {
		g_goMgr.EnemyNumSubtract();

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
}


void Game::GameDelete()
{
	//�Q�[�����̂��̂̃f���[�g�t���O�B
	//�Q�[���N���A�Ƃ��Q�[���I�[�o�[��
	//�V�[����]������Ƃ��ɌĂ΂�܂��B
	m_gameDelete = true;
}
