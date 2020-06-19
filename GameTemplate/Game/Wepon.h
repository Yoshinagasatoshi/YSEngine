#pragma once
#include "gameObject/ysGameObject.h"
/// <summary>
/// ����N���X
/// �N�i�C�┚�e�Ȃǂ̔�ѓ���n�̕���̋����̐e�ɂȂ�N���X
/// �����⑫�y�������Ă��镐��͌p�����Ȃ����ƁB
/// </summary>
class Enemy	;
class Wepon : public IGameObject
{
public:
	Wepon();
	~Wepon();
private:
	//��莞�Ԃ��Ə�����֐�
	virtual void DispTime();
	//���킲�Ƃ̍U��������o���֐��B
	virtual void WeponATKjuge();
	//�A�v�f
	virtual void Update();

	CVector3 SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	CVector3 SetScale(CVector3 scale)
	{
		m_scale = scale;
	}
	CQuaternion SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
protected:
	SkinModel m_skinModel;							//�X�L�����f���B
	CVector3 m_position = CVector3::Zero();
	CVector3 m_scale = CVector3::One();
	CQuaternion m_rotation = CQuaternion::Identity();
};

