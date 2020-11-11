#pragma once
#include "Player.h"
#include "Enemy.h"
/// <summary>
/// �Q�[���f�[�^�N���X
/// <remarks>
/// �Q�[���f�[�^�̃��X�g���ۑ������ق����ǂ����̂�ۑ�����N���X
/// </remarks>
/// </summary>
class GameData : public IGameObject
{
public:
	GameData();
	~GameData();
	void Update() {};
	void Draw() {};
	void RandomPos();
	/// <summary>
	/// �Q�[���f�[�^�̃C���X�^���X���擾
	/// </summary>
	/// <returns></returns>
	static GameData& GetInstans()
	{
		static GameData gamedata;
		return gamedata;
	}

	/// <summary>
	/// i�Ԗڂ̃X�|�b�g���擾����B
	/// </summary>
	CVector3 GetSpotPos(int i)
	{
		return EnemySpot[i];
	}
	/// <summary>
	/// i�Ԗڂ̃X�|�b�g���g���Ă��邩�H
	/// </summary>
	/// <param name="i"></param>
	bool GetSpotUse(int i)
	{
		if(i < 0 || i > kakoi_max){
			return nullptr;
		}
		return m_enclosure[i];
	}
	/// <summary>
	/// �ǂ̏ꏊ���g���Ă���̂�
	/// </summary>
	/// <param name="i"></param>
	void SetSpotUse(int i)
	{
		m_enclosure[i] = true;
	}

	/// <summary>
	/// ���̏ꏊ�͎g���Ă��Ȃ��B�Ƃ�������ݒ肷��
	/// </summary>
	/// <param name="i"></param>
	void RemoveSpotUse(int i)
	{
		m_enclosure[i] = false;
	}
	/// <summary>
	/// �͂��̍ő吔��Ԃ�
	/// </summary>
	/// <returns></returns>
	int GetKakoiNum()const
	{
		return kakoi_max;
	}
	/// <summary>
	/// �v���C���[�̏����Z�b�g����
	/// </summary>
	/// <param name="pl"></param>
	void SetPlayerInfo(Player* pl)
	{
		m_player = pl;
	}
private:
	static const int kakoi_max = 5;	//�͂��̍ő吔
	CVector3 EnemySpot[kakoi_max];	//�G�̏ꏊ�f�[�^
	Player* m_player;				//�v���C���[�̃|�C���^
	Enemy* m_enemy[kakoi_max];		//�G�l�~�[�̃f�[�^
	CVector3 CameraForword;			//�J�����̑O����
	CVector3 CameraRight;			//�J�����̉E����
	//�g�������͈͂̐��l
	const float kyori = 100.0f;		//�P�ʂƂ��Ďg������
	const float half = 50.0f;		//���P�ʂƂ��Ďg������
	//�ǂ̏ꏊ�̈͂����g���Ă��邩�B
	//enemy�^�̔z��ɂ��������s���������̂Őݒ�B
	bool m_enclosure[kakoi_max] = { false,false,false,false,false };
};

