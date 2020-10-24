/*
  ysGameObjectのマネージャー
*/
#pragma once
#include "gameObject/ysGameObject.h"
#include "util/Util.h"
	class ysGameObjectManager
	{
	public:
		ysGameObjectManager() {}
		~ysGameObjectManager() {}
		
		struct EffectTool
		{
			//Effekseerマネージャ管理。
			//インスタンスは1つ
			Effekseer::Manager* m_effekseerManager = nullptr;
			EffekseerRenderer::Renderer* m_effekseerRenderer = nullptr;

			//エフェクト
			Effekseer::Effect* m_sampleEffect = nullptr;
			Effekseer::Handle m_playEffectHandle = -1;
		};

		/// <summary>
		/// イフェクサーを入れる。
		/// </summary>
		void InitEffekseer();
		/// <summary>
		/// アップデート
		/// </summary>
		void Update();
		void XorY(int X);
		/// <summary>
		/// ゲームオブジェクトの名前キーを作成
		/// </summary>
		static unsigned int MakeGameObjectNameKey(const char* objectName)
		{
			static const unsigned int defaultNameKey = Util::MakeHash("Undefined");
			unsigned int hash;
			if (objectName == nullptr) {
				hash = defaultNameKey;
			}
			else {
				hash = Util::MakeHash(objectName);
			}
			return hash;
		}

		/// <summary>
		/// ゲームオブジェクトを作るクラス
		/// NewGO()を呼んでも結果的にはこの関数を通る
		/// </summary>
		template <class T>
		T* NewGameObject(const char* objectName)
		{
			T* newObj = new T;
			IGameObjectList.push_back(newObj);
			unsigned int hash = MakeGameObjectNameKey(objectName);
			newObj->m_nameKey = hash;
			return newObj;
		}

		/// <summary>
		/// ゲームオブジェクトを消す関数
		/// DeleteGO()を呼んでもこの関数を通ることになる
		/// IGameObjectを継承しているクラスを1つだけ消すときはこちらを使う
		/// 複数消したいときはDeleteGOObject"s"の方を使う。
		/// </summary>
		/// <param name="go"></param>
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
		/// <summary>
		/// DeleteGOOnjectの複数形
		/// 名前を検索して、その名前に該当するインスタンスを全て
		/// デリートする。
		/// </summary>
		/// <param name="go"></param>
		/// <param name="objname"></param>
		void DeleteGOObjects(IGameObject* go, const char * objname)
		{
			//リストから検索して
			for (auto it = IGameObjectList.begin();
				it != IGameObjectList.end();
				it++) {
				if ((*it) == go) {
					//削除リクエストを送る。

				}
			}
		}

		/// <summary>
		/// ゲームオブジェクト名(以下GO名)の検索。重い
		/// GO名の検索を1回だけ行いたいならこれを呼ぶ
		/// 同じ名前のGO名を複数呼びたい場合は
		/// FindGameObject"s"を呼ぶ。
		/// </summary>
		template<class T>
		T* FindGameObject(const char* objectName, bool enableErrorMessage = true)
		{
			unsigned int nameKey = Util::MakeHash(objectName);
			for (auto goList : IGameObjectList) {
				if (goList->m_nameKey == nameKey) {
					//発見
					T* p = dynamic_cast<T*>(goList);
					if (p == nullptr && enableErrorMessage == true) {
						//型変換に失敗
						//MessageBox("型変換に失敗しました");
						abort();
					}
					return p;
				}
			}
		}

		/// <summary>
		/// ゲームオブジェクト(以下GO)を複数検索したいときに使う関数
		/// </summary>
		template<class T>
		void FindGameObjects(const char* objectName, std::function<bool(T* go)> func)
		{
			unsigned int nameKey = Util::MakeHash(objectName);
			for (auto goList : IGameObjectList) {
				if (goList->m_nameKey == nameKey) {
					//見つけた。
					T* p = dynamic_cast<T*>(goList);
					if (func(p) == false) {
						//クエリ中断。
						return;
					}
				}
			}
		}

		/// <summary>
		/// 討伐数のカウント。呼ばれたら1足される
		/// </summary>
		void Counting()
		{
			Count++;
		}
		/// <summary>
		/// 何体討伐しているかを確認する関数
		/// </summary>
		/// <returns></returns>
		int GetCount() const
		{
			return Count;
		}
		/// <summary>
		/// 討伐数のカウントを0にするクラス。
		/// ゲームの2周目以降に呼ばれることを想定しています。
		/// </summary>
		void ResetCount()
		{
			Count = 0;
		}

		/// <summary>
		/// エネミーの数だけカウントする
		/// </summary>
		void EnemyCounting()
		{
			m_enemyCount++;
		}
		/// <summary>
		/// エネミーが死んだらカウントを減らす
		/// </summary>
		void EnemyNumSubtract()
		{
			m_enemyCount--;
		}
		///現在のエネミーの数を返す
		int GetEnemyNum() const
		{
			return  m_enemyCount;
		}
		///エネミーのカウントをリセットする
		void EnemyNumResetCount()
		{
			//?
			m_enemyCount = 0;
		}
		/// <summary>
		/// 呼ばれたらヒットストップをオンに
		/// </summary>
		void HitStopOn(int i)
		{
			m_isHitStopFrag = true;
			m_HitStopCount = i;
		}

		///呼ばれたらヒットストップをオフに
	/*	void HitStopOff()
		{
			m_isHitStopFrag = false;
		}*/
		//
		/// <summary>
		/// 現在のヒットストップの状態を返す
		/// </summary>
		/// <returns></returns>
		bool IsHitStop() const 
		{
			return m_isHitStopFrag;
		}

		/// <summary>
		/// 無双ゲージがどれだけあるかを返す関数。
		/// </summary>
		const float&GetMusouGaugeValue() const
		{
			return m_musouGauge;
		}
		/// <summary>
		/// 無双ゲージがどれだけ増減したかを受け取る関数
		/// 引数に-をいれたら減らせる。
		/// </summary>
		void AddMusouGauge(float Value)
		{
			m_musouGauge += Value;
			//もし、無双ゲージの最大値を超えそうなら
			if (m_musouGauge > 450.0f)
			{
				m_musouGauge = 450.0f;
			}
			//もし、無双ゲージの最低値を超えそうなら
			if (m_musouGauge < 0.0f)
			{
				m_musouGauge = 0.0f;
			}
		}

		/// <summary>
		/// 無双技が打てる状況かどうかを返す
		/// </summary>
		bool isMusouSpecial()const
		{
			return m_isMusouSpecial;
		}

		/// <summary>
		/// 無双技が打てる合図
		/// </summary>
		void SetMusou_Uteru()
		{
			m_isMusouSpecial = true;
		}

		/// <summary>
		/// 無双技が打てない合図
		/// </summary>
		void SetMusou_Utenai()
		{
			m_isMusouSpecial = false;
		}

		/// <summary>
		/// 呼ばれたら無双ゲージ量をリセットする。
		/// ゲームの2周目以降によばれる予定。
		/// addMusouGaugeでも代用はできる
		/// </summary>
		void ResetMusouGauge()
		{
			m_musouGauge = 0.0f;
		}

		//実行。ExecuteからDrawとUpdateに分離
		void Execute();
		//void Update();
		void Draw();

		//void DrawNoAlphaBlend();
		//2dも追加
		void Render();
		void Delete();
		void PostDraw()
		{
			for (auto go : IGameObjectList) {
				go->PostDraw();
			}
		}
		//配列数を取りたいのでリストを返すようにした
		std::list<IGameObject*> GetIGameObjectList()
		{
			return IGameObjectList;
		}
	private:
		//初期化。
		void Init(int gameObjectPropMax);

		void Start();
		//void Draw();
		void PostUpdate();
		void PostRender();

		void PreUpdate();

	private:
		//配列の数が変動したらカウントする変数を作りたかったので追加
		int Count = 0;
		//COuntとほぼ同じ、カウントとは別枠で数えたいものがあれば使う。名前変えるかも
		int m_enemyCount = 0;
		//クリア通知、届くようにしようか
		bool isCrear = false;
		//ゲームオーバー通知
		bool isGameOver = false;
		//ヒットストップをかけるかどうか、これがtrueになるとUpdate()が止まるように。
		bool m_isHitStopFrag = false;
		//Xにあたった時のヒットストップ書けるのか
		bool m_isHitXStopFrag = false;
		//ヒットストップタイマー;
		int HitStopTimer = 0;
		//ヒットストップのカウント
		int m_HitStopCount = 0;
		//無双ゲージの量を蓄積している変数
		float m_musouGauge;
		//無双技が打てるかどうかを判断する変数
		bool m_isMusouSpecial = false;
	private:
		//授業版格納庫
		std::list<IGameObject*> IGameObjectList; //ゲームオブジェクトのリスト
		//配列の格納庫
		typedef std::list<IGameObject*> GameObjectList;
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
	/// <summary>
	/// ゲームオブジェクト生成のヘルパー関数
	/// </summary>
	template<class T>
	static inline T* NewGO(const char* objectName)
	{
		return ysGameObjectManager().NewGameObject<T>(objectName);
	}
	/// <summary>
	/// ゲームオブジェクト削除のヘルパー関数
	/// NewGOを使用して作成したオブジェクトは必ずDeleteGOを実行すること
	/// </summary>
	/// <param name="go">削除するゲームオブジェクト</param>
	static inline void DeleteGO(IGameObject* go)
	{
		g_goMgr.DeleteGOObject(go);
	}
	/// <summary>
	/// ゲームオブジェクト検索のヘルパー関数
	/// 同名のゲームオブジェクト全てに対してQueryを行いたい場合に使用する。
	/// objectName	ゲームオブジェクトの名前
	/// func		ゲームオブジェクトが見つかった時に呼ばれるコールバック関数。
	/// <param name="objectName"></param>
	/// <param name="func"></param>
	/// </summary>
	template<class T>
	static inline void QueryGOs(const char* objectName, std::function<bool(T* go)>func)
	{
		return g_goMgr.FindGameObjects<T>(objectName,func);
	}
	/// <summary>
	/// ゲームオブジェクトを名前指定で削除
	/// </summary>
	static inline void DeleteGOs(const char* objectName)
	{
		QueryGOs<IGameObject>(objectName, [](auto go) {
			DeleteGO(go);
			return true;
		});
	}
	extern ysGameObjectManager::EffectTool g_Effect;