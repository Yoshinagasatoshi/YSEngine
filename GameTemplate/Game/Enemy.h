#pragma once
#include "gameObject/ysGameObject.h"
#include "Player.h"
/// <summary>
/// �G���N���X�B
/// <remarks>
/// �G�̊�{�I�ȓ�����^����N���X�ł�
/// ���y��G���N���X�̊��N���X�ɂȂ�\��
/// </remarks>
/// </summary>
class Player;
class GameCamera;
class Game;
class Enemy : public IGameObject
{
public:
	Enemy();
	virtual ~Enemy();
	//virtual void Update();
	//virtual void Draw();
	//virtual void Move() = 0;
	const CVector3& SetPos(const CVector3& position)
	{
		return m_position = position;
	}
	const CVector3& GetPos()const
	{
		return m_position;
	}

	CQuaternion SetRot(const CQuaternion qrot)
	{
		return m_rotation = qrot;
	}
	void SetPlayerInfo(Player* pl)
	{
		m_player = pl;
	}
	void SetGameCameraInfo(GameCamera* camera)
	{
		m_gameCamera = camera;
	}
	void SetGameinfo(Game* game)
	{
		m_game = game;
	}
	//�v���C���[�ƃG�l�~�[�̋������͂���
	//m_player�ɏ�񂪓����Ă��鎞��������
	void PlayertoDistans()
	{
		m_PtDistans = m_player->GetPosition() - m_position;
	}
	const CVector3& GetPlayertoDistans()const
	{
		return m_PtDistans;
	}

	//�����悤�Ȋ֐���asigaru�ɂ������ł����A
	//enemy�ɂ��ǉ����Ȃ���΂Ȃ�Ȃ��Ȃ��Ă��܂����݌v
	void ThisDeath()
	{
		m_isDeath = true;
	}

	bool GetenemyDeath()
	{
		return m_isDeath;
	}
protected:
	SkinModel m_model;			//�X�L�����f���B
	SkinModel m_model_Row;		//���[�|���̃X�L�����f���B�A�j���͂��Ȃ��\��
	CVector3 m_position;		//���ݒn
	CQuaternion m_rotation;		//��]�p�x
	CVector3 m_scale;
	CVector3 m_playerPos;		//�v���C���[�̃|�W�V����
	Player* m_player;			//�v���C���[�̃|�C���g�^
	GameCamera* m_gameCamera;	//�Q�[���J�����̃|�C���g�^�B
	Game* m_game;				//�Q�[��
	CVector3 m_PtDistans;		//�v���C���[�ƃG�l�~�[�̋���
	const float grabity = -9.8f * 2.0f;//�d��
	const float BattleRange = 200.0f * 200.0f;			//���̋����͈͓̔��ɋ߂Â��ƃo�g��
	const float VigilanceRange = 700.0f * 700.0f;		//���̋����͈͓̔��Ȃ�v���C���[�ɋ߂Â��B

	bool m_isDeath = false;
};

