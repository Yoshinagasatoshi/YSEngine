#pragma once
#include "graphics/Sprite.h"
#include "gameObject/ysGameObject.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
/// <summary>
/// タイトルクラス
/// ボタンを押されるとシーン遷移が行われるシンプルなクラスです
/// </summary>
class Title : public IGameObject
{
public:
	Title();
	~Title();
	void Update();
	void Draw();
private:
	Sprite m_sprite;									//画像データ
	bool m_isWaitFadeOut = false;						//フェードアウトが完了したかどうか

	CSoundEngine m_soundEngine;							//タイトルで再生するためのサウンドエンジン
	CSoundSource m_se;									//効果音
	CSoundSource m_bgm;									//タイトルで流すＢＧＭ
};

