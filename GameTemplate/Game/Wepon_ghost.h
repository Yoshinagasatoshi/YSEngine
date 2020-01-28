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
	//eventName�͂����Ɠǂݍ��܂�Ă��邪�A
	//������^�𔻒f�ɂ���̂�����̂�
	//int�^�Ƃ��ɂ�������������������Ȃ�
	//�Ƃ肠�����ۗ��̃R�����g�A�E�g�B
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
	//�󂯎����CVector3�̂�[��ʂ̓z�ɒu�������Ă݂�
	CQuaternion m_rotation = CQuaternion::Identity();
	CVector3 m_scale = CVector3::One();
	int timer = 0;
	//char eventName;
	bool m_hanteifin = false;
};

