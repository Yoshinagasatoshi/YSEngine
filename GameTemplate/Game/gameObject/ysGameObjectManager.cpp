#include "stdafx.h"
#include "ysGameObjectManager.h"


namespace ysEngine {
	void ysGameObjectManager::Start()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->StartWrapper();
			}
		}
	}

	void ysGameObjectManager::Update()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->UpdateWrapper();
			}
		}
	}

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

	void ysGameObjectManager::Execute()
	{
		//StartŠÖ”‚Ìˆ—‚©‚çn‚Ü‚éB
		Start();
		//Update
		Update();
	}

	void ysGameObjectManager::Init(int gameObjectPrioMax)
	{
		
	}
}