/*
  ysGameObjectのマネージャー
*/
#pragma once
#include "gameObject/ysGameObject.h"
	class ysGameObjectManager
	{
	public:
		ysGameObjectManager() {}
		~ysGameObjectManager() {}
		//アップデート
		void Update();
		//newGO・放課後版
		template <class T>
		T* NewGameObject()
		{
			T* newObj = new T;
			IGameObjectList.push_back(newObj);
			return newObj;
		}
		//デリート・放課後版
		void DeleteGOObject(IGameObject* go)
		{
			//リストから検索して、見つかったら削除する。
			for (auto it = IGameObjectList.begin();
				it != IGameObjectList.end();
				it++) {
				if ((*it) == go) {
					//削除リクエストを送る。
					go->RequestDelete();
				}
			}
		}
		//実行。
		void Execute();
	private:
		
		//初期化。
		void Init(int gameObjectPropMax);
		
	private:
		void Start();
		void Draw();
		void PostUpdate();
		void PreUpdate();
		
		
	private:
		//授業版格納庫
		std::vector<IGameObject*> IGameObjectList; //ゲームオブジェクトのリスト
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
	//外部アクセスをするので、extern宣言がいる。
	extern ysGameObjectManager g_goMgr;