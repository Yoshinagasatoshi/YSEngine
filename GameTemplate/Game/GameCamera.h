#pragma once
#include "Player.h"
class Player;
class GameCamera : public IGameObject
{
public:
	/// <summary>
	/// 射影行列の更新方法。
	/// </summary>
	enum EnUpdateProjMatrixFunc {
		enUpdateProjMatrixFunc_Perspective,		//!<透視射影行列。遠近法が効いた絵を作りたいならこっち。
		enUpdateProjMatrixFunc_Ortho,			//!<平行投影。２Ｄ的な表現がしたいならこっち。
	};
	GameCamera();
	~GameCamera();
	void Update();
	void Draw();
	void SetPlayerInfo(Player* pl)
	{
		m_player = pl;
	}
	CVector3 GetCameraPos()
	{
		return m_position;
	}
private:
	CVector3 m_position;								//カメラ位置
	CQuaternion m_rotation = CQuaternion::Identity();	//カメラの回転角度
	CVector3 m_AxisY = CVector3{ 0.0f,1.0f,0.0f };		//Ｙ軸
	CVector3 m_target;									//カメラ注視点
	CVector3 m_toPos = CVector3{0,250,400};				//カメラの位置と注視点の距離
	CVector3 m_up = CVector3::Up();						//上方向
	float m_viewAngle = CMath::DegToRad(60.0f);			//画角。
	//CMatrix m_matrix;
	Player* m_player;									//プレイヤーのポインタ
};

