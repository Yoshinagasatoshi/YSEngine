#include "stdafx.h"
#include "InGameSoundDirector.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
const float MIN_BGM_VOL = 0.3f;		//BGM���ʂ̉����l
const float MAGNIFICATION = 0.1f;	//�{���B���̐��l�𒲐�����ƌ��ʉ����鎞�ɂǂꂾ��BGM�̉��ʂ��������Ȃ邩���ς��
const float RIGGZERO = 0;			//���ʉ��̂Ȃ��Ă��鐔���[���Ȃ�
InGameSoundDirector::InGameSoundDirector()
{
	
}
InGameSoundDirector::~InGameSoundDirector()
{
	m_Swing.Release();
	m_Swing2.Release();
	m_Slash.Release();
	m_Down.Release();
	m_fuse.Release();
	m_Destruct.Release();
	m_Kick1.Release();
	m_Kick2.Release();
	m_zoriRun.Release();
	m_onigiri.Release();
	m_houkai.Release();
}

void InGameSoundDirector::Update()
{
	if (m_bgm.IsPlaying()) {
		//BGM���炳��Ă���Ȃ�ȉ��̏���������

		float subtraction = m_seRingCount * MAGNIFICATION;	//m_seRingCount�caddringnum()���Ă΂ꂽ���������l��������ϐ�
		float BGM_finalVol = BGM_NormalVol - subtraction;
		//BGM���������Ȃ肷���Ȃ��悤�ɂ���
		if (BGM_finalVol < MIN_BGM_VOL)
		{
			BGM_finalVol = MIN_BGM_VOL;//MinBGM��0.5�������Ă���A����ȉ��ɂ͂Ȃ�Ȃ�
		}
		m_bgm.SetVolume(BGM_finalVol);//�ŏI�I�ȉ��ʂ�BGM�𗬂��B
	}

		if (m_seRingCount >= RIGGZERO) {
			if (!m_Slash.IsPlaying()) {
				//SE���Đ����I�����̂Ȃ�J�E���g�����炷�B
				m_seRingCount--;
			}
			else if (!m_Slash.IsPlaying()) {
				//SE���Đ����I�����̂Ȃ�J�E���g�����炷�B
				m_seRingCount--;
			}
			else if (!m_Slash.IsPlaying()) {
				//SE���Đ����I�����̂Ȃ�J�E���g�����炷�B
				m_seRingCount--;
			}
			else if (!m_fuse.IsPlaying()) {
				//SE���Đ����I�����̂Ȃ�J�E���g�����炷�B
				m_seRingCount--;
			}
			else if (!m_Destruct.IsPlaying()) {
				//SE���Đ����I�����̂Ȃ�J�E���g�����炷�B
				m_seRingCount--;
			}
			else if (!m_onigiri.IsPlaying()) {
				//SE���Đ����I�����̂Ȃ�J�E���g�����炷�B
				m_seRingCount--;
			}
			else if (!m_houkai.IsPlaying()) {
				//SE���Đ����I�����̂Ȃ�J�E���g�����炷�B
				m_seRingCount--;
			}
		}
	g_soundEngine->Update();
}


void InGameSoundDirector::BGMtyousei()
{

}

