#include "stdafx.h"
#include "GameCamera.h"
#include "gameObject/ysGameObjectManager.h"

const float TARGET_Y_UP_VALUE = 100.0f;//������ύX����ƃJ�����̒����_�̍������ς��B
const float RotAmount = 5.0f; //��]�̑��x�P��
const float Camera_Y_MAX = 0.9f;//�J�������
const float Camera_Y_MIN = 0.1f;//�J����������

GameCamera::GameCamera()
{
	
}
GameCamera::~GameCamera()
{
	g_goMgr.DeleteGOObject(this);
}
void GameCamera::Update()
{
	//�����_�̌v�Z
	m_target = m_player->GetPosition();
	CVector3 m_toCameraPosOld = m_toPos;
	//�p�b�h�̓���
	float stickx = g_pad->GetRStickXF();
	//float sticky = g_pad->GetRStickYF();
	//Y���ł̉�]
	CQuaternion qRot;
	//�x�N�g���N���X��낤
	qRot.SetRotationDeg(m_AxisY, RotAmount * stickx);
	qRot.Multiply(m_toPos);
	//X���ł̉�]
	/*CVector3 axisX;
	axisX.Cross(CVector3::AxisY, m_toPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 2.0f * y);
	qRot.Multiply(m_toPos);*/
	//�J�����̃x�N�g����ۑ�

	//�J������]�̏�������߂�B
	CVector3 toPosDir = m_toPos;
	toPosDir.Normalize();
	if (toPosDir.y < Camera_Y_MIN) {//�J������y��0.1�ȉ��̂Ƃ�(�n�ʂ̕����Ɍ����Ȃ��悤�ɂ����)
		m_toPos = m_toCameraPosOld;
	}
	else if (toPosDir.y > Camera_Y_MAX) {//�J������y��0,9�ȏ�̎�(�J�������^������������Ȃ��悤��)
		m_toPos = m_toCameraPosOld;
	}
	//���_�̌v�Z
	m_position = m_target + m_toPos;
	//Pos.y += 50.0f;
	//�^�[�Q�b�g��������������
	m_target.y += TARGET_Y_UP_VALUE;
	//���C���J�����̒����_�Ǝ��_��ݒ肷��B
	g_camera3D.SetTarget(m_target);
	g_camera3D.SetPosition(m_position);
	g_camera3D.Update();

	
}
void GameCamera::Draw()
{

}