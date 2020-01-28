#pragma once
#include "physics/PhysicsGhostObject.h"
#include "gameObject/ysGameObject.h"
class Wepon_ghost : public IGameObject
{
public:
	Wepon_ghost();
	~Wepon_ghost();
	void Update();
	bool Start();
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	//eventNameはちゃんと読み込まれているが、
	//文字列型を判断にするのが難しいので
	//int型とかにした方がいいかもしれない
	//とりあえず保留のコメントアウト。
	//void eventName(char* eventname)
	//{
	//	eventName = eventname;
	//}
	PhysicsGhostObject* GetGhostObject() {
		return &m_ghostObject;
	}
	void GhostInit();

private:
	PhysicsGhostObject m_ghostObject;
	CVector3 m_position;
	//受け取ったCVector3のやーつを別の奴に置き換えてみる
	CQuaternion m_rotation = CQuaternion::Identity();
	CVector3 m_scale = CVector3::One();
	int timer = 0;
	//char eventName;
	bool m_hanteifin = false;
};

