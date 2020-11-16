#pragma once
#include "graphics/Sprite.h"
#include "gameObject/ysGameObject.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
/// <summary>
/// ゲームオーバーになった時に呼ばれるクラス
/// プレイヤーが死ぬかタイムオーバーになった時によばれっる
/// </summary>
class GameOver : public IGameObject
{
public:
	GameOver();
	~GameOver();

	void Update();
	void PostDraw();
	void Draw();
	enum animClipNumber {
		anim_Lose,
		animClip_num
	};
private:

	Sprite m_sprite2; //一つ目のスプライト
	//Sprite m_sprite2; //二つ目のスプライト
	SkinModel m_skinModel;//やられたプレイヤーを移す用。
	SkinModel m_skinModelStage;	//ステージ用のモデル

	CVector3 m_position = CVector3::Zero();				//場所
	CVector3 m_scale = CVector3::One();					//大きさ
	CQuaternion m_rot = CQuaternion::Identity();		//回転角度
	CVector2 m_pivot = { 0.5f,0.5f };					//起点
	AnimationClip m_animClip[animClip_num];				//アニメーションクリップ
	Animation m_animeModel;								//アニメモデルよう

	CSoundEngine m_soundEngine;							//サウンドエンジン
	CSoundSource m_se;									//効果音
	CSoundSource m_bgm;									//ＢＧＭ
	bool m_isWaitFadeOut = false;						//フェードが終わったかどうかのフラグ
};

