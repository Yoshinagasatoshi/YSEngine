#include "stdafx.h"
#include "Camera.h"

Camera g_camera3D;		//3D�J�����B
Camera g_camera2D;		//2D�J�����B
Camera g_cameraMap;		//2D�}�b�v�p�J����

void Camera::Update()
{
	
	//�r���[�s����v�Z�B
	m_viewMatrix.MakeLookAt(
		m_position,
		m_target,
		m_up
	);
	//�r���[�s��̋t�s����v�Z�B
	//����ŃJ�����s��ɂȂ�
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
	//�v���W�F�N�V�����s����v�Z�B
	m_projMatrix.MakeProjectionMatrix(
		m_viewAngle,					//��p�B
		FRAME_BUFFER_W / FRAME_BUFFER_H,	//�A�X�y�N�g��B
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
	//-600�����傤�ǂ�������
	//g_camera2D.SetPosition({ 0.0f, 0.0f, camera2Dpos_z });
	//g_camera2D.SetTarget(CVector3::Zero());
	//g_camera2D.Update();
}
