#pragma once
#include "physics/PhysicsGhostObject.h"
#include "gameObject/ysGameObject.h"
/// <summary>
/// 武器判定を作るクラス
/// </summary>
class Player;
class Wepon_ghost : public IGameObject
{
public:
	Wepon_ghost();
	~Wepon_ghost();
	void Update();
	//bool Start();
	/// <summary>
	/// 場所の設定
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	//void eventName(char* eventname)
	//{
	//	eventName = eventname;
	//}
	/// <summary>
	/// プレイヤーの情報をセットする
	/// 引数はポインタで渡す事
	/// </summary>
	/// <param name="pl"></param>
	void SetPlayerInfo(Player* pl)
	{
		m_player = pl;
	}
	/// <summary>
	/// プレイヤーから呼ばれたことを
	/// 判断する
	/// </summary>
	void Playercall()
	{
		m_playerCall = true;
	}
	//const
	/// <summary>
	/// Ghostオブジェクトを取得する
	/// </summary>
	/// <returns></returns>
	PhysicsGhostObject* GetGhostObject() {
		return &m_ghostObject;
	}
	//ゴーストの初期化
	void GhostInit();
private:
	PhysicsGhostObject m_ghostObject;					//ゴーストオブジェクト
	CVector3 m_position;								//発生場所
	CQuaternion m_rotation = CQuaternion::Identity();	//発生角度
	CVector3 m_scale = CVector3::One();					//ghostのサイズを設定。* 100.0f
	bool m_hanteifin = false;							//判定が終わったか
	Player* m_player;									//プレイヤーのポインタ
	bool m_PLSLASH = false;								//プレイヤーが斬ったか
	float m_ghostTimer = 0;								//ゴーストが出る時間		
	bool m_playerCall = false;							//プレイヤーが呼んだものか
};

