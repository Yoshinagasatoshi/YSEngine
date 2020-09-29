#include "stdafx.h"
#include "Camera.h"

Camera g_camera3D;		//3Dカメラ。
Camera g_camera2D;		//2Dカメラ。
Camera g_cameraMap;		//2Dマップ用カメラ

void Camera::Update()
{
	
	//ビュー行列を計算。
	m_viewMatrix.MakeLookAt(
		m_position,
		m_target,
		m_up
	);
	//ビュー行列の逆行列を計算。
	//これでカメラ行列になる
	m_viewMatrixInv.Inverse(m_viewMatrix);

	m_forward.Set(	
		m_viewMatrixInv.m[2][0],
		m_viewMatrixInv.m[2][1],
		m_viewMatrixInv.m[2][2]
	);
	m_right.Set(	
		m_viewMatrixInv.m[0][0],
		m_viewMatrixInv.m[0][1],
		m_viewMatrixInv.m[0][2]
	);
	//プロジェクション行列を計算。
	m_projMatrix.MakeProjectionMatrix(
		m_viewAngle,					//画角。
		FRAME_BUFFER_W / FRAME_BUFFER_H,	//アスペクト比。
		m_near,
		m_far
	);
	
}

void Camera::InitCamera()
{
	const float camera2Dpos_z = -600.0f;

	//g_camera3D.Update();

	g_camera2D.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	//g_camera2D.SetWidth(FRAME_BUFFER_W);
	//g_camera2D.SetHeight(FRAME_BUFFER_H);
	//-600がちょうどいい感じ
	//g_camera2D.SetPosition({ 0.0f, 0.0f, camera2Dpos_z });
	//g_camera2D.SetTarget(CVector3::Zero());
	//g_camera2D.Update();
}
