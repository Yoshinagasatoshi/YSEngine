#pragma once
#include "gameObject/ysGameObject.h"
#include "graphics/Sprite.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
/// <summary>
/// �Q�[���N���A�N���X
/// �G������|�����Ƃ��ɌĂ΂��
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
	void PostDraw();		//�x��ăh���[
	void Draw();
	SkinModel m_skinModel;//���������v���C���[���ڂ��p�B
	SkinModel m_skinModelStage;//���U���g�p�X�e�[�W���f�����ڂ�
	Sprite m_sprite1; //��ڂ̃X�v���C�g
	Sprite m_sprite2; //��ڂ̃X�v���C�g
	CVector3 m_position = CVector3::Zero();	//�ꏊ
	CVector3 m_scale = CVector3::One();		//�傫��
	CQuaternion m_rot = CQuaternion::Identity();//��]
	CVector2 m_pivot = { 0.5f,0.5f };		//�N�_
	AnimationClip m_animClip[animClip_num];	//�A�j���[�V�����̃N���b�v
	Animation m_animeModel;					//�A�j���[�V������K�������郂�f��

	CSoundEngine m_soundEngine;					//���U���g�p�̃T�E���h�G���W��	
	CSoundSource m_se;							//���ʉ�
	CSoundSource m_bgm;							//���U���gBGM_Clear

	bool m_isWaitFadeOut = false;				//�t�F�[�h�A�E�g�������������ǂ����̃t���O
};

