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
		//デリート
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
		//配列
		template <class T>
		T* FindGOObject(IGameObject* go)
		{
			//配列カウント
			int hairetunumber = 0;
			//リストから検索して、見つかったら返す
			for (auto it = IGameObjectList.begin();
				it != IGameObjectList.end();
				it++) {
				i++;
				if ((*it) == go) {
					T* p = dynamic_cast<T*>(go);
					if (p == nullptr && enableErrorMessage == true) {
						//型変換に失敗
						MessageBox("型変換に失敗しました");
					}
					return p;
				}
			}
		}
		//findGO?先生のやつをまねたけど
		template<class T>
		T* FindGameObject(const char* objactName, bool enableErrorMessage)
		{
			unsigned int nameKey = CUtil::MakeHash(ObjectName);
			for (auto go : goList) {
				if (go->m_nameKey == nameKey) {
					//発見
					T* p = dynamic_cast<T*>(go);
					if (p == nullptr && enableErrorMessage == true) {
						//型変換に失敗
						MessageBox("型変換に失敗しました");
					}
					return p;
				}
			}
		}
		//実行。ExecuteからDrawとUpdateに分離
		void Execute();
		void Updater();
		void Draw();
	private:
		//初期化。
		void Init(int gameObjectPropMax);
		
	private:
		void Start();
		//void Draw();
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