#pragma once
#include "Wepon.h"
#include "Enemy.h"
#include "physics/PhysicsGhostObject.h"
class Player;
class bom : public Wepon
{
public:
	bom();
	~bom();
	void Draw();
	void Update();
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	void SetInpactPosition(CVector3 plPos)
	{
		m_inpactPoint = plPos;
	}
	void SetPlayerInfo(Player* player)
	{
		m_player = player;
	}
private:
	void FirstSet();				//�{���̏����ݒ�
	void Finalbom();				//�{�������e�n�_�t�������ɌĂԊ֐�
	void HitThebom();				//�{�����v���C���[�ɓ����������ĂԊ֐�
	bool m_isFirst = false;
	bool m_isSecond = false;
	CVector3 m_inpactPoint;//���n�_
	CVector3 m_ReleyPoint;//���p�_
	CVector3 m_bomGhostPos;//�{���̃|�W�V�������C������p�̕ϐ�
	Player* m_player;
	CVector3 m_bomVelocity;//�{���̑���
	PhysicsGhostObject m_ghostObject;				//����̓����蔻��
};

