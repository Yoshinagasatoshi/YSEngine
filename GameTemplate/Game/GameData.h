#pragma once
#include "Player.h"
#include "Enemy.h"
/// <summary>
/// ゲームデータクラス
/// <remarks>
/// ゲームデータのリスト等保存したほうが良いものを保存するクラス
/// </remarks>
/// </summary>
class GameData : public IGameObject
{
public:
	GameData();
	~GameData();
	void Update() {};
	void Draw() {};
	void RandomPos();
	/// <summary>
	/// ゲームデータのインスタンスを取得
	/// </summary>
	/// <returns></returns>
	static GameData& GetInstans()
	{
		static GameData gamedata;
		return gamedata;
	}

	/// <summary>
	/// i番目のスポットを取得する。
	/// </summary>
	CVector3 GetSpotPos(int i)
	{
		return EnemySpot[i];
	}
	/// <summary>
	/// i番目のスポットが使われているか？
	/// </summary>
	/// <param name="i"></param>
	bool GetSpotUse(int i)
	{
		if(i < 0 || i > kakoi_max){
			return nullptr;
		}
		return m_enclosure[i];
	}
	/// <summary>
	/// どの場所を使っているのか
	/// </summary>
	/// <param name="i"></param>
	void SetSpotUse(int i)
	{
		m_enclosure[i] = true;
	}

	/// <summary>
	/// この場所は使われていない。という事を設定する
	/// </summary>
	/// <param name="i"></param>
	void RemoveSpotUse(int i)
	{
		m_enclosure[i] = false;
	}
	/// <summary>
	/// 囲いの最大数を返す
	/// </summary>
	/// <returns></returns>
	int GetKakoiNum()const
	{
		return kakoi_max;
	}
	/// <summary>
	/// プレイヤーの情報をセットする
	/// </summary>
	/// <param name="pl"></param>
	void SetPlayerInfo(Player* pl)
	{
		m_player = pl;
	}
private:
	static const int kakoi_max = 5;	//囲いの最大数
	CVector3 EnemySpot[kakoi_max];	//敵の場所データ
	Player* m_player;				//プレイヤーのポインタ
	Enemy* m_enemy[kakoi_max];		//エネミーのデータ
	CVector3 CameraForword;			//カメラの前方向
	CVector3 CameraRight;			//カメラの右方向
	//使いたい範囲の数値
	const float kyori = 100.0f;		//単位として使いたい
	const float half = 50.0f;		//半単位として使いたい
	//どの場所の囲いが使われているか。
	//enemy型の配列にした方が都合がいいので設定。
	bool m_enclosure[kakoi_max] = { false,false,false,false,false };
};

