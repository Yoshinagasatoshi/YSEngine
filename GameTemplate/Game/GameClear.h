#pragma once
#include "gameObject/ysGameObject.h"
#include "graphics/Sprite.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
/// <summary>
/// ゲームクリアクラス
/// 敵武将を倒したときに呼ばれる
/// </summary>
class GameClear : public IGameObject
{
public:
	GameClear();
	~GameClear();
	enum animClipNumber {
		anim_Win,
		animClip_num
	};
private:
	void Update();	
	void PostDraw();		//遅れてドロー
	void Draw();
	SkinModel m_skinModel;//勝利したプレイヤーを移す用。
	SkinModel m_skinModelStage;//リザルト用ステージモデルを移す
	Sprite m_sprite1; //一つ目のスプライト
	Sprite m_sprite2; //二つ目のスプライト
	CVector3 m_position = CVector3::Zero();	//場所
	CVector3 m_scale = CVector3::One();		//大きさ
	CQuaternion m_rot = CQuaternion::Identity();//回転
	CVector2 m_pivot = { 0.5f,0.5f };		//起点
	AnimationClip m_animClip[animClip_num];	//アニメーションのクリップ
	Animation m_animeModel;					//アニメーションを適応させるモデル

	CSoundEngine m_soundEngine;					//リザルト用のサウンドエンジン	
	CSoundSource m_se;							//効果音
	CSoundSource m_bgm;							//リザルトBGM_Clear

	bool m_isWaitFadeOut = false;				//フェードアウトが完了したかどうかのフラグ
};

