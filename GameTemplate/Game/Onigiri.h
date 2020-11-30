#pragma once
#include "gameObject/ysGameObject.h"
/// <summary>
/// ���ɂ���N���X
/// �ĕU�N���X���U���ɂ���Ĕj�󂳂ꂽ��o��
/// �񕜃A�C�e���̃N���X
/// </summary>
class Player;
class UI;
class Onigiri : public IGameObject
{
public:
	Onigiri();
	~Onigiri();

	void GetAitem();			//�A�C�e�����擾�����Ƃ��ɌĂ΂��
	void SetPlayer(Player* PL)			//�v���C���[�����Z�b�g
	{
		m_player = PL;
	}
	void SetPosition(CVector3 pos)//�ꏊ���Z�b�g
	{
		m_position = pos;
	}
private:
	void Update();
	void Draw();
	SkinModel m_onigiriModel;	//���ɂ���̃��f���B�񕜃A�C�e���Ƃ��Ďg�p

	CVector3 m_position;						//���ɂ��胂�f���̈ʒu
	CQuaternion m_rot = CQuaternion::Identity();//���ɂ��胂�f���̉�]
	CVector3 m_scale = CVector3::One();			//���ɂ��胂�f���̑傫��

	float m_rotValue = 0.0f;					//��]�ʂ̒l
	Player* m_player;							//�v���C���[�̃|�C���^
};

