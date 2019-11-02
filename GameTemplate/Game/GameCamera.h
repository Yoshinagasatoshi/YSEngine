#pragma once
#include "Player.h"
class Player;
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	void Update();
	void Draw();
	void SetPlayerInfo(Player* pl)
	{
		m_player = pl;
	}
private:
	//CVector3 m_position = { 0.0f, 100.0f, 300.0f };
	CQuaternion m_rotation = CQuaternion::Identity();
	CVector3 m_AxisY = CVector3{ 0.0f,1.0f,0.0f };
	CVector3 m_target;
	CVector3 m_toPos = CVector3{0,250,400};
	//CMatrix m_matrix;
	Player* m_player;

};
