#include "stdafx.h"
#include "ysGameObjectManager.h"
//ysGameObjectManagerクラスのインスタンス。
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
	//	//登録されているゲームオブジェクトの更新処理を呼ぶ。
	//	for (auto go : IGameObjectList) {
	//		go->Update();
	//		go->Draw();
	//	}

		//全てのゲームオブジェクトの1フレーム分の処理が終わってから、削除する。
		for (auto it = IGameObjectList.begin(); it != IGameObjectList.end();) {
			if ((*it)->isRequestDelete()) {
				//削除リクエストを受けているので削除
				delete* it;
				it = IGameObjectList.erase(it);
			}
			else {
				//リクエストを受けていないため
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