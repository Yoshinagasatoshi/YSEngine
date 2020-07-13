#pragma once
#include "gameObject/ysGameObjectManager.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
//���ASE�����Ȃ��Ă��邩���v�����āA����𗬂����ǂ��������߂�N���X
//���ꂾ���̂��߂ɍ��ꂽ
//�c�\�肾�������ǁABGM�̉��Ƃ��������ɒS�����Ă��炤���Ƃɂ��悤
class SoundDirector: public IGameObject
{
public:

	SoundDirector();
	~SoundDirector();

	//�ŏ��ɋN������
	//�R���X�g���N�^�ɂ��Ă��������Ǌ֐��ɂ����B
	void SoundInit()
	{
		//�T�E���h�G���W�����������B����Ă邱�Ƃ͉���
		g_soundEngine->Init();
	}

	void Update();

	////BGM��炷
	void InGameStartUpBGM()
	{
		//��SE�f��
		m_bgm.Init(L"Assets/sound/Chanbara.wav");
		m_bgm.Play(true);
		//se�`�F�b�N�̂��߂ɁA�啪������
		m_bgm.SetVolume(1.0f);
	}

	static SoundDirector& GetInstans()
	{
		//���񋟂��Ă����ȃA�c
		static SoundDirector SDdata;
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

	////SE��炵�܂�
	void RingSE()
	{
		m_Swing1.Init(L"Assets/sound/swing.wav");
		m_Swing2.Init(L"Assets/sound/swing.wav");
		if (m_Swing1.IsPlaying()) {
			m_Swing1.Play(false);
			m_Swing1.SetVolume(3.0f);//�����ɂł������Ă���B��Œ���
		}
		else {
			if (m_Swing2.IsPlaying()) {
				m_Swing2.Play(false);
				m_Swing2.SetVolume(3.0f);//�����ɂł������Ă���B��Œ���
			}
		}

	}
private:
	//SE���ǂꂾ���Ȃ��Ă��邩���J�E���g����B
	int m_seRingCount = 0;
	//�ҋ@�҂���se���ǂꂾ�����邩���J�E���g����B
	int m_seStockCount = 0;
	//�v���C���[�̕��Ɍ��X��������
	//SoundDirector�N���X�ŃQ�[�����̉��y��S���Ǘ����邱�Ƃɂ����B
	CSoundSource m_bgm;
	CSoundSource m_Swing1;
	CSoundSource m_Swing2;
	
};

