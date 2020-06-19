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
	void FirstSet();				//ボムの初期設定
	void Finalbom();				//ボムが着弾地点付いた時に呼ぶ関数
	void HitThebom();				//ボムがプレイヤーに当たった時呼ぶ関数
	bool m_isFirst = false;
	bool m_isSecond = false;
	CVector3 m_inpactPoint;//着地点
	CVector3 m_ReleyPoint;//中継点
	CVector3 m_bomGhostPos;//ボムのポジションを修正する用の変数
	Player* m_player;
	CVector3 m_bomVelocity;//ボムの速さ
	PhysicsGhostObject m_ghostObject;				//武器の当たり判定
};

