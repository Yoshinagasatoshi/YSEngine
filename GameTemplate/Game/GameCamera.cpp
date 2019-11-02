#include "stdafx.h"
#include "GameCamera.h"
GameCamera::GameCamera()
{
	
}
GameCamera::~GameCamera()
{

}
void GameCamera::Update()
{
	//注視点の計算
	m_target = m_player->GetPosition();
	CVector3 m_toCameraPosOld = m_toPos;
	//パッドの入力
	float x = g_pad->GetRStickXF();
	float y = g_pad->GetRStickYF();
	//Y軸での回転
	CQuaternion qRot;
	//ベクトルクラス作ろう
	qRot.SetRotationDeg(m_AxisY, 2.0 * x);
	qRot.Multiply(m_toPos);
	//X軸での回転
	/*CVector3 axisX;
	axisX.Cross(CVector3::AxisY, m_toPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 2.0f * y);
	qRot.Multiply(m_toPos);*/
	//カメラのベクトルを保存

	//マジックナンバーがあるため後で0.1fとかを変数名に変えたい。
	//カメラ回転の上限を決める。
	CVector3 toPosDir = m_toPos;
	toPosDir.Normalize();
	if (toPosDir.y < 0.1f) {
		m_toPos = m_toCameraPosOld;
	}
	else if (toPosDir.y > 0.9f) {
		m_toPos = m_toCameraPosOld;
	}
	//視点の計算
	CVector3 Pos = m_target + m_toPos;
	//メインカメラの注視点と視点を設定する。
	g_camera3D.SetTarget(m_target);
	g_camera3D.SetPosition(Pos);
	g_camera3D.Update();
}
void GameCamera::Draw()
{

}