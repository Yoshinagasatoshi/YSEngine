#pragma once
#include "gameObject/ysGameObjectManager.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
//今、SEが何個なっているかを計測して、それを流すかどうかを決めるクラス
//BGMの音とかもこいつに担当してもらうことに
class InGameSoundDirector: public IGameObject
{
public:

	InGameSoundDirector();
	~InGameSoundDirector();

	//最初に起こそう
	//コンストラクタにしてもいいけど関数にした。
	void SoundInit()
	{
		//サウンドエンジンを初期化。
		g_soundEngine->Init();
	}

	void Update();

	////BGMを鳴らす・ゲーム内
	void InGameStartUpBGM()
	{
		//音SE素材
		m_bgm.Init(L"Assets/sound/Chanbara.wav");
		m_bgm.Play(true);
		//seボリュームを引数にセット
		m_bgm.SetVolume(BGM_NormalVol);
	}

	static InGameSoundDirector& GetInstans()
	{
		static InGameSoundDirector SDdata;
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

	//SE_素振り音
	void RingSE_Swing()
	{
		if (m_seRingCount< Max_RingNum) {
			if (m_Swing.IsPlaying()) {
				m_Swing2.Init(L"Assets/sound/swing.wav");
				m_Swing2.Play(false);
				m_Swing2.SetVolume(3.0f);//試しにでかくしている。後で調整
			}
			else {
				m_Swing.Init(L"Assets/sound/swing.wav");
				m_Swing.Play(false);
				m_Swing.SetVolume(3.0f);//試しにでかくしている。後で調整
			}
			addringnum();
		}
	}
	//SE_斬撃音
	void RingSE_Slash()
	{
		if (m_seRingCount < Max_RingNum) {
			m_Slash.Init(L"Assets/sound/slash1.wav");//鳴らす音のデータの初期化
			m_Slash.Play(false);//再生。falseは一度だけ再生する。
			m_Slash.SetVolume(3.0f);//効果音の音の量。
			addringnum();//効果音が鳴っている数を+1する。
		}
	}
	//SE_倒れる音
	void RingSE_Down() 
	{
		if (m_seRingCount < Max_RingNum) {
			m_Down.Init(L"Assets/sound/falldown2.wav");
			m_Down.Play(false);
			m_Down.SetVolume(1.5f);
			addringnum();
		}
	}
	//SE_導火線の音
	void RingSE_Fuse()
	{
		if (m_seRingCount < Max_RingNum) {
			m_fuse.Init(L"Assets/sound/fuse.wav");
			m_fuse.Play(false);
			m_fuse.SetVolume(1.5f);
			addringnum();
		}
	}
	//SE_爆発の音
	void RingSE_Destruct()
	{
		if (m_seRingCount < Max_RingNum) {
			m_Destruct.Init(L"Assets/sound/destruction.wav");
			m_Destruct.Play(false);
			m_Destruct.SetVolume(3.0f);
			addringnum();
		}
	}
	
	//SE_けり音
	void RingSE_Kick()
	{
		if (m_seRingCount < Max_RingNum) {
			if (m_Swing.IsPlaying()) {
				m_Swing2.Init(L"Assets/sound/RotaryKick.wav");
				m_Swing2.Play(false);
				m_Swing2.SetVolume(3.0f);//試しにでかくしている。後で調整
			}
			else {
				m_Kick1.Init(L"Assets/sound/RoundHouseKick.wav");
				m_Kick1.Play(false);
				m_Kick1.SetVolume(3.0f);//試しにでかくしている。後で調整
			}
			addringnum();
		}
	}
	//Updateをオンにする
	void UpdateOn()
	{
		m_isUpdate = true;
	}
	//Updateをオフにする
	void UpdateOff()
	{
		m_isUpdate = false;
	}
	//Updateの状態を確認する。
	bool GetisUpdateMode()
	{
		return m_isUpdate;
	}

private:
	//SEがどれだけなっているかをカウントする。
	int m_seRingCount = 0;
	//待機待ちのseがどれだけあるかをカウントする。
	int m_seStockCount = 0;
	//ならせる効果音の上限数
	const int Max_RingNum = 3;

	//SoundDirectorクラスでゲーム内の音楽を全部管理することにした。
	CSoundSource m_bgm;	//ゲーム中にかかっているBGM。タイトルとゲーム内で切り替わる
	CSoundSource m_bgm2;	//ゲーム中にかかっているBGM。タイトルとゲーム内で切り替わる
	CSoundSource m_bgm3;	//ゲーム中にかかっているBGM。タイトルとゲーム内で切り替わる

	const float BGM_NormalVol = 1.0f;//通常時のBGM音
	CSoundSource m_Swing;//剣を振る音
	CSoundSource m_Swing2;//剣を振る音2
	CSoundSource m_Slash;//斬る音
	CSoundSource m_Down;//倒れる音
	CSoundSource m_fuse;//導火線の音
	CSoundSource m_Destruct;//爆発の音
	CSoundSource m_Kick1;//けり音1
	CSoundSource m_Kick2;//けり音2

	bool m_isUpdate = false;//アプデ関数呼ぶかどうか。
};

