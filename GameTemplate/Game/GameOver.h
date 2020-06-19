#pragma once
#include "graphics/Sprite.h"
#include "gameObject/ysGameObject.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
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

	Sprite m_sprite2; //��ڂ̃X�v���C�g
	//Sprite m_sprite2; //��ڂ̃X�v���C�g
	SkinModel m_skinModel;//���ꂽ�v���C���[���ڂ��p�B
	SkinModel m_skinModelStage;

	CVector3 m_position = CVector3::Zero();
	CVector3 m_scale = CVector3::One();
	CQuaternion m_rot = CQuaternion::Identity();
	CVector2 m_pivot = { 0.5f,0.5f };
	AnimationClip m_animClip[animClip_num];
	Animation m_animeModel;

	CSoundEngine m_soundEngine;
	CSoundSource m_se;									//���ʉ�
	CSoundSource m_bgm;
	bool m_isWaitFadeOut = false;
};

