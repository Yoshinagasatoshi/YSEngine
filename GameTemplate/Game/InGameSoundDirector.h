#pragma once
#include "gameObject/ysGameObjectManager.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
//���ASE�����Ȃ��Ă��邩���v�����āA����𗬂����ǂ��������߂�N��
//BGM�̉��Ƃ��������ɒS�����Ă��炤���Ƃɂ��悤
class InGameSoundDirector: public IGameObject
{
public:

	InGameSoundDirector();
	~InGameSoundDirector();

	//�ŏ��ɋN������
	//�R���X�g���N�^�ɂ��Ă��������Ǌ֐��ɂ����B
	void SoundInit()
	{
		//�T�E���h�G���W�����������B����Ă邱�Ƃ͉���
		g_soundEngine->Init();
	}

	void Update();

	////BGM��炷�E�Q�[����
	void InGameStartUpBGM()
	{
		//��SE�f��
		m_bgm.Init(L"Assets/sound/Chanbara.wav");
		m_bgm.Play(true);
		//se�`�F�b�N�̂��߂ɁA�啪������
		m_bgm.SetVolume(BGM_NormalVol);
	}

	static InGameSoundDirector& GetInstans()
	{
		static InGameSoundDirector SDdata;
		return SDdata;
	}
	//������Ă��鉹�𑫂�
	void addringnum()
	{
		m_seRingCount++;
	}
	//������Ă��鉹��n��
	int GetReturnRingNum()
	{
		return m_seRingCount;
	}
	//
	void mainasuRingnum()
	{
		m_seRingCount--;
	}

	//BGM��SE�̒��a��ڎw���B
	void BGMtyousei();

	//�����\�肪����\��̓z�𑫂�
	void addstocknumS()
	{
		m_seStockCount++;
	}

	void mainasustocknumS()
	{
		m_seStockCount--;
	}

	//�ҋ@���̉����ǂꂾ�����邩
	int GetReturnRingNumS()
	{
		return m_seStockCount;
	}

	//SE_�f�U�艹
	void RingSE_Swing()
	{
		if (m_Swing.IsPlaying()) {
			m_Swing2.Init(L"Assets/sound/swing.wav");
			m_Swing2.Play(false);
			m_Swing2.SetVolume(3.0f);//�����ɂł������Ă���B��Œ���
		}
		else {
			m_Swing.Init(L"Assets/sound/swing.wav");
			m_Swing.Play(false);
			m_Swing.SetVolume(3.0f);//�����ɂł������Ă���B��Œ���
		}
		addringnum();
	}
	//SE_�a����
	void RingSE_Slash()
	{
		m_Slash.Init(L"Assets/sound/slash1.wav");
		m_Slash.Play(false);
		m_Slash.SetVolume(3.0f);//�����ɂł������Ă���B��Œ���
		addringnum();
	}
	//SE_�|��鉹
	void RingSE_Down() 
	{
		m_Down.Init(L"Assets/sound/falldown2.wav");
		m_Down.Play(false);
		m_Down.SetVolume(1.5f);
		addringnum();
	}
	//SE_���ΐ��̉�
	void RingSE_Fuse()
	{
		m_fuse.Init(L"Assets/sound/fuse.wav");
		m_fuse.Play(false);
		m_fuse.SetVolume(1.5f);
		addringnum();
	}
	//SE_�����̉�
	void RingSE_Destruct()
	{
		m_Destruct.Init(L"Assets/sound/destruction.wav");
		m_Destruct.Play(false);
		m_Destruct.SetVolume(3.0f);
		addringnum();
	}
	//Update���I���ɂ���
	void UpdateOn()
	{
		m_isUpdate = true;
	}
	//Update���I�t�ɂ���
	void UpdateOff()
	{
		m_isUpdate = false;
	}
	//Update�̏�Ԃ��m�F����B
	bool GetisUpdateMode()
	{
		return m_isUpdate;
	}

private:
	//SE���ǂꂾ���Ȃ��Ă��邩���J�E���g����B
	int m_seRingCount = 0;
	//�ҋ@�҂���se���ǂꂾ�����邩���J�E���g����B
	int m_seStockCount = 0;

	//SoundDirector�N���X�ŃQ�[�����̉��y��S���Ǘ����邱�Ƃɂ����B
	CSoundSource m_bgm;	//�Q�[�����ɂ������Ă���BGM�B�^�C�g���ƃQ�[�����Ő؂�ւ��
	CSoundSource m_bgm2;	//�Q�[�����ɂ������Ă���BGM�B�^�C�g���ƃQ�[�����Ő؂�ւ��
	CSoundSource m_bgm3;	//�Q�[�����ɂ������Ă���BGM�B�^�C�g���ƃQ�[�����Ő؂�ւ��

	const float BGM_NormalVol = 1.0f;//�ʏ펞��BGM��
	CSoundSource m_Swing;//����U�鉹
	CSoundSource m_Swing2;//����U�鉹2
	CSoundSource m_Slash;//�a�鉹
	CSoundSource m_Down;//�|���
	CSoundSource m_fuse;//���ΐ��̉�
	CSoundSource m_Destruct;//���������̉�

	bool m_isUpdate = false;//�A�v�f�֐��ĂԂ��ǂ���
};

