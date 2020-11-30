#pragma once
#include "gameObject/ysGameObject.h"
/// <summary>
/// �ĕU�N���X
/// �̗͂��񕜂����邽�߂̃A�C�e�����o���N���X
/// �v���C���[�̍U���ɂ���Ĕj�󂳂��
/// �܂�A�v���C���[���U�������Ƃ����M�����󂯎��Ȃ���΂����Ȃ�
/// </summary>
class Player;
class Onigiri;
class Komedawara : public IGameObject
{
public:
	Komedawara();
	~Komedawara();
	/// <summary>
	/// ���ݒn�̐ݒ�
	/// </summary>
	/// <param name="pos"></param><returns></returns>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// ��]�̐ݒ�
	/// </summary>
	/// <param name="qrot"></param>
	/// <returns></returns>
	void SetRot(CQuaternion qrot)
	{
		m_rot = qrot;
	}
	/// <summary>
	/// �傫���̐ݒ�
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	void SetScale(CVector3 scale)
	{
		m_scale = scale;
	}
	void SetPlayerInfo(Player* PL)
	{
		m_player = PL;
	}
private:
	void Update();
	void Draw();

	CVector3 m_position = CVector3::Zero();	//���f���ʒu
	CQuaternion m_rot = CQuaternion::Identity();//���f����]
	CVector3 m_scale = CVector3::One();		//���f���̑傫��

	Player*	m_player;				//�v���C���[�̃|�C���^
	Onigiri* m_onigiri;				//�񕜃A�C�e���̃|�C���^
	SkinModel m_komedawaraModel;//�ĕU�̃��f��
};

