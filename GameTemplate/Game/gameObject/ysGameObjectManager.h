/*
  ysGameObjectのマネージャー
*/
#pragma once
#include "gameObject/ysGameObject.h"
namespace ysEngine{
	class ysGameObjectManager
	{
	public:
		ysGameObjectManager();
		~ysGameObjectManager();
	private:
		//実行。
		void Execute();
		//初期化。
		void Init(int gameObjectPropMax);
		
	private:
		void Start();
		void Update();
		void PostUpdate();
		void PreUpdate();
	private:
		//配列の格納庫
		typedef std::vector<IGameObject*> GameObjectList;
		//Listはつけない
		/// <summary>
		/// NewGO用のリスト。tkEngineと違い優先度はない。
		/// </summary>
		std::vector<GameObjectList> m_gameObjectListArray;
		/// <summary>
		/// DeleteGO用のリスト
		/// </summary>
		std::vector<GameObjectList> m_deleteObjectArray[2];
	};
};
