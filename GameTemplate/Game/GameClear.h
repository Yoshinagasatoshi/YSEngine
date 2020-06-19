#pragma once
#include "gameObject/ysGameObject.h"
#include "graphics/Sprite.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
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
	void PostDraw();
	void Draw();
	SkinModel m_skinModel;//���ꂽ�v���C���[���ڂ��p�B
	SkinModel m_skinModelStage;
	Sprite m_sprite1; //��ڂ̃X�v���C�g
	Sprite m_sprite2; //��ڂ̃X�v���C�g
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

