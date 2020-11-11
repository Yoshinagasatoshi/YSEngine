#pragma once
#include "Player.h"
class Player;
class GameCamera : public IGameObject
{
public:
	/// <summary>
	/// �ˉe�s��̍X�V���@�B
	/// </summary>
	enum EnUpdateProjMatrixFunc {
		enUpdateProjMatrixFunc_Perspective,		//!<�����ˉe�s��B���ߖ@���������G����肽���Ȃ炱�����B
		enUpdateProjMatrixFunc_Ortho,			//!<���s���e�B�Q�c�I�ȕ\�����������Ȃ炱�����B
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
	CVector3 m_position;								//�J�����ʒu
	CQuaternion m_rotation = CQuaternion::Identity();	//�J�����̉�]�p�x
	CVector3 m_AxisY = CVector3{ 0.0f,1.0f,0.0f };		//�x��
	CVector3 m_target;									//�J���������_
	CVector3 m_toPos = CVector3{0,250,400};				//�J�����̈ʒu�ƒ����_�̋���
	CVector3 m_up = CVector3::Up();						//�����
	float m_viewAngle = CMath::DegToRad(60.0f);			//��p�B
	//CMatrix m_matrix;
	Player* m_player;									//�v���C���[�̃|�C���^
};

