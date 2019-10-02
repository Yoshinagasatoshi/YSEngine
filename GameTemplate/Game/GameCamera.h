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
	CVector3 m_position = { 0.0f, 100.0f, 300.0f };
	CQuaternion m_rotation = CQuaternion::Identity();
	//CMatrix m_matrix;
	Player* m_player;
};

