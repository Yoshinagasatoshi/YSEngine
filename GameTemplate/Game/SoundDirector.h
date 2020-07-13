#pragma once
#include "gameObject/ysGameObjectManager.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
//今、SEが何個なっているかを計測して、それを流すかどうかを決めるクラス
//それだけのために作られた
//…予定だったけど、BGMの音とかもこいつに担当してもらうことにしよう
class SoundDirector: public IGameObject
{
public:

	SoundDirector();
	~SoundDirector();

	//最初に起こそう
	//コンストラクタにしてもいいけど関数にした。
	void SoundInit()
	{
		//サウンドエンジンを初期化。やってることは遠回し
		g_soundEngine->Init();
	}

	void Update();

	////BGMを鳴らす
	void InGameStartUpBGM()
	{
		//音SE素材
		m_bgm.Init(L"Assets/sound/Chanbara.wav");
		m_bgm.Play(true);
		//seチェックのために、大分下げる
		m_bgm.SetVolume(1.0f);
	}

	static SoundDirector& GetInstans()
	{
		//空を提供してるんやろなア…
		static SoundDirector SDdata;
		return SDdata;
	}
	//流されている音を足す
	void addringnum()
	{
		m_seRingCount++;
	}
	//流されている音を渡す
	int GetReturnRingNum()
	{
		return m_seRingCount;
	}
	//
	void mainasuRingnum()
	{
		m_seRingCount--;
	}

	//BGMとSEの調和を目指す。
	void BGMtyousei();

	//流す予定がある予定の奴を足す
	void addstocknumS()
	{
		m_seStockCount++;
	}

	void mainasustocknumS()
	{
		m_seStockCount--;
	}

	//待機中の音がどれだけあるか
	int GetReturnRingNumS()
	{
		return m_seStockCount;
	}

	////SEを鳴らします
	void RingSE()
	{
		m_Swing1.Init(L"Assets/sound/swing.wav");
		m_Swing2.Init(L"Assets/sound/swing.wav");
		if (m_Swing1.IsPlaying()) {
			m_Swing1.Play(false);
			m_Swing1.SetVolume(3.0f);//試しにでかくしている。後で調整
		}
		else {
			if (m_Swing2.IsPlaying()) {
				m_Swing2.Play(false);
				m_Swing2.SetVolume(3.0f);//試しにでかくしている。後で調整
			}
		}

	}
private:
	//SEがどれだけなっているかをカウントする。
	int m_seRingCount = 0;
	//待機待ちのseがどれだけあるかをカウントする。
	int m_seStockCount = 0;
	//プレイヤーの方に元々あったが
	//SoundDirectorクラスでゲーム内の音楽を全部管理することにした。
	CSoundSource m_bgm;
	CSoundSource m_Swing1;
	CSoundSource m_Swing2;
	
};

