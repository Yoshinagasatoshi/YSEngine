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
	//�����_�̌v�Z
	m_target = m_player->GetPosition();
	CVector3 m_toCameraPosOld = m_toPos;
	//�p�b�h�̓���
	float x = g_pad->GetRStickXF();
	float y = g_pad->GetRStickYF();
	//Y���ł̉�]
	CQuaternion qRot;
	//�x�N�g���N���X��낤
	qRot.SetRotationDeg(m_AxisY, 2.0 * x);
	qRot.Multiply(m_toPos);
	//X���ł̉�]
	/*CVector3 axisX;
	axisX.Cross(CVector3::AxisY, m_toPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 2.0f * y);
	qRot.Multiply(m_toPos);*/
	//�J�����̃x�N�g����ۑ�

	//�}�W�b�N�i���o�[�����邽�ߌ��0.1f�Ƃ���ϐ����ɕς������B
	//�J������]�̏�������߂�B
	CVector3 toPosDir = m_toPos;
	toPosDir.Normalize();
	if (toPosDir.y < 0.1f) {
		m_toPos = m_toCameraPosOld;
	}
	else if (toPosDir.y > 0.9f) {
		m_toPos = m_toCameraPosOld;
	}
	//���_�̌v�Z
	CVector3 Pos = m_target + m_toPos;
	//���C���J�����̒����_�Ǝ��_��ݒ肷��B
	g_camera3D.SetTarget(m_target);
	g_camera3D.SetPosition(Pos);
	g_camera3D.Update();
}
void GameCamera::Draw()
{

}