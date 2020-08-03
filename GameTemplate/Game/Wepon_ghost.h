#pragma once
#include "physics/PhysicsGhostObject.h"
#include "gameObject/ysGameObject.h"
class Player;
class Wepon_ghost : public IGameObject
{
public:
	Wepon_ghost();
	~Wepon_ghost();
	void Update();
	//bool Start();
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	//void eventName(char* eventname)
	//{
	//	eventName = eventname;
	//}
	void SetPlayerInfo(Player* pl)
	{
		m_player = pl;
	}
	void Playercall()
	{
		m_playerCall = true;
	}
	PhysicsGhostObject* GetGhostObject() {
		return &m_ghostObject;
	}
	void GhostInit();
private:
	PhysicsGhostObject m_ghostObject;
	CVector3 m_position;
	CQuaternion m_rotation = CQuaternion::Identity();
	//ghostのサイズを設定。* 100.0f
	CVector3 m_scale = CVector3::One();
	//char eventName;
	bool m_hanteifin = false;
	Player* m_player;
	bool m_PLSLASH = false;
	float m_ghostTimer = 0;	//ゴーストが出る時間
	bool m_playerCall = false;
};

