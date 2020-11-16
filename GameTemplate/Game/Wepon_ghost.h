#pragma once
#include "physics/PhysicsGhostObject.h"
#include "gameObject/ysGameObject.h"
/// <summary>
/// ���픻������N���X
/// </summary>
class Player;
class Wepon_ghost : public IGameObject
{
public:
	Wepon_ghost();
	~Wepon_ghost();
	void Update();
	//bool Start();
	/// <summary>
	/// �ꏊ�̐ݒ�
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	//void eventName(char* eventname)
	//{
	//	eventName = eventname;
	//}
	/// <summary>
	/// �v���C���[�̏����Z�b�g����
	/// �����̓|�C���^�œn����
	/// </summary>
	/// <param name="pl"></param>
	void SetPlayerInfo(Player* pl)
	{
		m_player = pl;
	}
	/// <summary>
	/// �v���C���[����Ă΂ꂽ���Ƃ�
	/// ���f����
	/// </summary>
	void Playercall()
	{
		m_playerCall = true;
	}
	//const
	/// <summary>
	/// Ghost�I�u�W�F�N�g���擾����
	/// </summary>
	/// <returns></returns>
	PhysicsGhostObject* GetGhostObject() {
		return &m_ghostObject;
	}
	//�S�[�X�g�̏�����
	void GhostInit();
private:
	PhysicsGhostObject m_ghostObject;					//�S�[�X�g�I�u�W�F�N�g
	CVector3 m_position;								//�����ꏊ
	CQuaternion m_rotation = CQuaternion::Identity();	//�����p�x
	CVector3 m_scale = CVector3::One();					//ghost�̃T�C�Y��ݒ�B* 100.0f
	bool m_hanteifin = false;							//���肪�I�������
	Player* m_player;									//�v���C���[�̃|�C���^
	bool m_PLSLASH = false;								//�v���C���[���a������
	float m_ghostTimer = 0;								//�S�[�X�g���o�鎞��		
	bool m_playerCall = false;							//�v���C���[���Ă񂾂��̂�
};

