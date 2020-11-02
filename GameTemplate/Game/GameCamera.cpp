#include "stdafx.h"
#include "GameCamera.h"
#include "gameObject/ysGameObjectManager.h"

const float TARGET_Y_UP_VALUE = 100.0f;//ここを変更するとカメラの注視点の高さが変わる。
const float RotAmount = 5.0f; //回転の速度単位
const float Camera_Y_MAX = 0.9f;//カメラ上限
const float Camera_Y_MIN = 0.1f;//カメラかげん

GameCamera::GameCamera()
{
	
}
GameCamera::~GameCamera()
{
	g_goMgr.DeleteGOObject(this);
}
void GameCamera::Update()
{
	//注視点の計算
	m_target = m_player->GetPosition();
	CVector3 m_toCameraPosOld = m_toPos;
	//パッドの入力
	float stickx = g_pad->GetRStickXF();
	//float sticky = g_pad->GetRStickYF();
	//Y軸での回転
	CQuaternion qRot;
	//ベクトルクラス作ろう
	qRot.SetRotationDeg(m_AxisY, RotAmount * stickx);
	qRot.Multiply(m_toPos);
	//X軸での回転
	/*CVector3 axisX;
	axisX.Cross(CVector3::AxisY, m_toPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 2.0f * y);
	qRot.Multiply(m_toPos);*/
	//カメラのベクトルを保存

	//カメラ回転の上限を決める。
	CVector3 toPosDir = m_toPos;
	toPosDir.Normalize();
	if (toPosDir.y < Camera_Y_MIN) {//カメラのyが0.1以下のとき(地面の方向に向かないようにする為)
		m_toPos = m_toCameraPosOld;
	}
	else if (toPosDir.y > Camera_Y_MAX) {//カメラのyが0,9以上の時(カメラが真上を向きすぎないように)
		m_toPos = m_toCameraPosOld;
	}
	//視点の計算
	m_position = m_target + m_toPos;
	//Pos.y += 50.0f;
	//ターゲットを少し高くする
	m_target.y += TARGET_Y_UP_VALUE;
	//メインカメラの注視点と視点を設定する。
	g_camera3D.SetTarget(m_target);
	g_camera3D.SetPosition(m_position);
	g_camera3D.Update();

	
}
void GameCamera::Draw()
{

}