#include "stdafx.h"
#include "InGameSoundDirector.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
const float MinBGMVol = 0.5f;
InGameSoundDirector::InGameSoundDirector()
{
	
}
InGameSoundDirector::~InGameSoundDirector()
{

}

void InGameSoundDirector::Update()
{
		if (m_bgm.IsPlaying()) {
			float a = m_seRingCount * 0.1f;
			float BGM_finalVol = BGM_NormalVol - a;
			//BGM���������Ȃ肷���Ȃ��悤�ɂ���
			if (BGM_finalVol < MinBGMVol)
			{
				BGM_finalVol = MinBGMVol;
			}
			m_bgm.SetVolume(BGM_finalVol);
		}

		if (m_seRingCount >= 0) {
			if (!m_Slash.IsPlaying()) {
				//SE���Đ����I�����̂Ȃ�J�E���g�����炷�B
				m_seRingCount--;
			}
			if (!m_Slash.IsPlaying()) {
				//SE���Đ����I�����̂Ȃ�J�E���g�����炷�B
				m_seRingCount--;
			}
			if (!m_Slash.IsPlaying()) {
				//SE���Đ����I�����̂Ȃ�J�E���g�����炷�B
				m_seRingCount--;
			}
			if (!m_fuse.IsPlaying()) {
				//SE���Đ����I�����̂Ȃ�J�E���g�����炷�B
				m_seRingCount--;
			}
			if (!m_Destruct.IsPlaying()) {
				//SE���Đ����I�����̂Ȃ�J�E���g�����炷�B
				m_seRingCount--;
			}
		}
	g_soundEngine->Update();
}


void InGameSoundDirector::BGMtyousei()
{

}

