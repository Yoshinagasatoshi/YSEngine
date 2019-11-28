#include "stdafx.h"
#include "ysGameObjectManager.h"
//ysGameObjectManager�N���X�̃C���X�^���X�B
ysGameObjectManager g_goMgr;

	void ysGameObjectManager::Start()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->StartWrapper();
			}
		}
	}

	//void ysGameObjectManager::Update()
	//{
	//	for (GameObjectList objList : m_gameObjectListArray) {
	//		for (IGameObject* obj : objList) {
	//			obj->UpdateWrapper();
	//		}
	//	}
	//}

	void ysGameObjectManager::PostUpdate()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->PostUpdateWrapper();
			}
		}
	}

	void ysGameObjectManager::PreUpdate()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->PreUpdateWrapper();
			}
		}
	}

	//void ysGameObjectManager::Draw()
	//{
	//	for (GameObjectList objList : m_gameObjectListArray) {
	//		for (IGameObject* obj : objList) {
	//			obj->DrawWrapper();
	//		}
	//	}
	//}

	void ysGameObjectManager::Updater()
	{
		for (auto go : IGameObjectList) {
			go->Update();
		}
	}

	void ysGameObjectManager::Draw()
	{
		for (auto go : IGameObjectList) {
			go->Draw();
		}
	}
	void ysGameObjectManager::Execute()
	{
	//	//�o�^����Ă���Q�[���I�u�W�F�N�g�̍X�V�������ĂԁB
	//	for (auto go : IGameObjectList) {
	//		go->Update();
	//		go->Draw();
	//	}

		//�S�ẴQ�[���I�u�W�F�N�g��1�t���[�����̏������I����Ă���A�폜����B
		for (auto it = IGameObjectList.begin(); it != IGameObjectList.end();) {
			if ((*it)->isRequestDelete()) {
				//�폜���N�G�X�g���󂯂Ă���̂ō폜
				delete* it;
				it = IGameObjectList.erase(it);
			}
			else {
				//���N�G�X�g���󂯂Ă��Ȃ�����
				it++;
			}
		}
	}

	void ysGameObjectManager::Init(int gameObjectPrioMax)
	{
		IGameObjectList.reserve(gameObjectPrioMax);
		m_gameObjectListArray.resize(gameObjectPrioMax);
		m_deleteObjectArray[0].resize(gameObjectPrioMax);
		m_deleteObjectArray[1].resize(gameObjectPrioMax);
	}