#pragma once
#include "graphics/Sprite.h"
#include "gameObject/ysGameObject.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
/// <summary>
/// �Q�[���I�[�o�[�ɂȂ������ɌĂ΂��N���X
/// �v���C���[�����ʂ��^�C���I�[�o�[�ɂȂ������ɂ�΂����
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

	Sprite m_sprite2; //��ڂ̃X�v���C�g
	//Sprite m_sprite2; //��ڂ̃X�v���C�g
	SkinModel m_skinModel;//���ꂽ�v���C���[���ڂ��p�B
	SkinModel m_skinModelStage;	//�X�e�[�W�p�̃��f��

	CVector3 m_position = CVector3::Zero();				//�ꏊ
	CVector3 m_scale = CVector3::One();					//�傫��
	CQuaternion m_rot = CQuaternion::Identity();		//��]�p�x
	CVector2 m_pivot = { 0.5f,0.5f };					//�N�_
	AnimationClip m_animClip[animClip_num];				//�A�j���[�V�����N���b�v
	Animation m_animeModel;								//�A�j�����f���悤

	CSoundEngine m_soundEngine;							//�T�E���h�G���W��
	CSoundSource m_se;									//���ʉ�
	CSoundSource m_bgm;									//�a�f�l
	bool m_isWaitFadeOut = false;						//�t�F�[�h���I��������ǂ����̃t���O
};

