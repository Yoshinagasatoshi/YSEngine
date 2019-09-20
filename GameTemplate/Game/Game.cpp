#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "BackGround.h"
#include "level/Level.h"
#include "gameObject/ysGameObjectManager.h"
//�R���X�g���N�^���Ă΂��ƃ��x���ŃL������\��������悤�ɂ��Ă���
Game::Game()
{
	//�v���C���[
	Player* player = g_goMgr.NewGameObject<Player>();
	BackGround* layer = g_goMgr.NewGameObject<BackGround>();
	Level level;
	level.Init(L"Assets/level/musou_stage.tkl",
		[&](const LevelObjectData& objdata) {
		//�z�u����Ă���I�u�W�F�N�g��asigaru�Ȃ�true
		//wcscmp�͕�����̔�r���s���֐��B
			if (wcscmp(objdata.name, L"asigaru") == 0) {
				//�C���X�^���X�̍쐬
				Enemy* enemy = g_goMgr.NewGameObject<Enemy>();
				enemy->SetPos(objdata.position);
				enemy->SetPlayer(player);
				//�ϒ��z��Ɂ��̃C���X�^���X��ǉ�
				return true;
			}
		});
	//Enemy enemy = new Enemy;
	//m_Player->SetEnemy(enemy);
}

Game::~Game()
{

}

void Game::Update()
{
	
}