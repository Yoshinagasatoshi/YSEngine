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
	level.Init(L"Assets/level/musou_siken.tkl",
		[&](const LevelObjectData& objdata) {
		//�z�u����Ă���I�u�W�F�N�g��asigaru�Ȃ�true
		//wcscmp�͕�����̔�r���s���֐��B
			if (wcscmp(objdata.name, L"asigaru") == 0) {
				//�C���X�^���X�̍쐬
				Enemy_asigaru* enemy = g_goMgr.NewGameObject<Enemy_asigaru>();
				enemy->SetPos(objdata.position);
				//enemy->SetRot(objdata.rotation);
				enemy->SetPlayerInfo(m_player);
				enemy->SetGameDataInfo(m_gamedata);
				//�ϒ��z��Ɂ��̃C���X�^���X��ǉ�
				return true;
			}
		});
	
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
	
}