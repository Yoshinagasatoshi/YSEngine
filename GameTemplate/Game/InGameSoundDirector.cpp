#include "stdafx.h"
#include "InGameSoundDirector.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
const float MinBGMVol = 0.4f;

InGameSoundDirector::InGameSoundDirector()
{
	
}
InGameSoundDirector::~InGameSoundDirector()
{
	/*m_Swing;
	m_Swing2;
	m_Slash;
	m_Down;
	m_fuse;;
	m_Destruct;
	m_Kick1;
	m_Kick2;
	m_zoriRun;*/
}

void InGameSoundDirector::Update()
{
	if (m_bgm.IsPlaying()) {
		//BGM���炳��Ă���Ȃ�ȉ��̏���������

		float subtraction = m_seRingCount * 0.1f;	//m_seRingCount�caddringnum()���Ă΂ꂽ���������l��������ϐ�
		float BGM_finalVol = BGM_NormalVol - subtraction;
		//BGM���������Ȃ肷���Ȃ��悤�ɂ���
		if (BGM_finalVol < MinBGMVol)
		{
			BGM_finalVol = MinBGMVol;//MinBGM��0.5�������Ă���A����ȉ��ɂ͂Ȃ�Ȃ�
		}
		m_bgm.SetVolume(BGM_finalVol);//�ŏI�I�ȉ��ʂ�BGM�𗬂��B
	}

		if (m_seRingCount >= 0) {
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
		}
	g_soundEngine->Update();
}


void InGameSoundDirector::BGMtyousei()
{

}

