#pragma once
#include "gameObject/ysGameObject.h"
/// <summary>
/// �G���N���X�B
/// <remarks>
/// �G�̊�{�I�ȓ�����^����N���X�ł�
/// ���y��G���N���X�̊��N���X�ɂȂ�\��
/// </remarks>
/// </summary>
class Enemy : public IGameObject
{
public:
	Enemy();
	~Enemy();
	void Update();
	void Draw();
private:
	SkinModel m_model;	//�X�L�����f���B
	CVector3 m_position = CVector3{ 100.0f,-100.0f,0.0f }; //�G�̈ʒu�A�ł����x���Ŕz�u����̂ŉ�
	CQuaternion m_rotation = CQuaternion{ 0.0f,0.0f,0.0f,1.0f };//�����̃��x��
	CVector3 m_scale = CVector3::One(); //�G�̑傫�����������x����
};

