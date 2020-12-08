#include "stdafx.h"
#include "InGameSoundDirector.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
const float MIN_BGM_VOL = 0.3f;		//BGM音量の下限値
const float MAGNIFICATION = 0.1f;	//倍率。この数値を調整すると効果音が鳴る時にどれだけBGMの音量が小さくなるかが変わる
const float RIGGZERO = 0;			//効果音のなっている数がゼロなら
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
		//BGMが鳴らされているなら以下の処理をする

		float subtraction = m_seRingCount * MAGNIFICATION;	//m_seRingCount…addringnum()が呼ばれた数だけ数値が増える変数
		float BGM_finalVol = BGM_NormalVol - subtraction;
		//BGMが小さくなりすぎないようにする
		if (BGM_finalVol < MIN_BGM_VOL)
		{
			BGM_finalVol = MIN_BGM_VOL;//MinBGMは0.5が入っており、これ以下にはならない
		}
		m_bgm.SetVolume(BGM_finalVol);//最終的な音量のBGMを流す。
	}

		if (m_seRingCount >= RIGGZERO) {
			if (!m_Slash.IsPlaying()) {
				//SEが再生し終えたのならカウントを減らす。
				m_seRingCount--;
			}
			else if (!m_Slash.IsPlaying()) {
				//SEが再生し終えたのならカウントを減らす。
				m_seRingCount--;
			}
			else if (!m_Slash.IsPlaying()) {
				//SEが再生し終えたのならカウントを減らす。
				m_seRingCount--;
			}
			else if (!m_fuse.IsPlaying()) {
				//SEが再生し終えたのならカウントを減らす。
				m_seRingCount--;
			}
			else if (!m_Destruct.IsPlaying()) {
				//SEが再生し終えたのならカウントを減らす。
				m_seRingCount--;
			}
			else if (!m_onigiri.IsPlaying()) {
				//SEが再生し終えたのならカウントを減らす。
				m_seRingCount--;
			}
			else if (!m_houkai.IsPlaying()) {
				//SEが再生し終えたのならカウントを減らす。
				m_seRingCount--;
			}
		}
	g_soundEngine->Update();
}


void InGameSoundDirector::BGMtyousei()
{

}

