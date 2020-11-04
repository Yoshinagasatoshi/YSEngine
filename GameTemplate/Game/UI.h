#pragma once
#include "graphics/Sprite.h"
#include "gameObject/ysGameObject.h"
#include "Font.h"
//UI�N���X�B�̗̓o�[���̉摜�𒣂�
class Player;
class Game;
class UI : public IGameObject
{
public:
	UI();
	~UI();
	void Update();
	void PostDraw();
	void SetPlayerInfo(Player* pl)
	{
		m_player = pl;
	}
	void SetGameInfo(Game* game)
	{
		m_game = game;
	}
private:
	Sprite m_lifeGauge;
	Sprite m_lifeGaugeura;

	Sprite m_musouGauge;
	Sprite m_musouGaugeura;

	Sprite m_face;

	Sprite m_mapSprite;

	Sprite m_playerPointer;
	Sprite m_playerPointer_yazirushi;


	//sprite1 �̗̓Q�[�W�̃f�[�^
	CVector3 m_position = CVector3{0.0f,-280.0f,0.0f};
	CQuaternion m_rotation = CQuaternion::Identity();
	CVector3 m_scale = CVector3::One();
	CVector2 m_pivot = { 0.0f,0.5f };

	//sprite2
	CVector3 m_position2 = CVector3{ -60.0f,-280.0f,0.0f };
	CQuaternion m_rotation2 = CQuaternion::Identity();
	CVector3 m_scale2 = CVector3::One();
	CVector2 m_pivot2 = { 0.5f,0.5f };

	//�}�b�v
	CVector3 m_position3 = CVector3{ 380.0f,240.0f,0.0f };
	CQuaternion m_rotation3 = CQuaternion::Identity();
	CVector3 m_scale3 = CVector3::One();
	CVector2 m_pivot3 = { 0.5f,0.5f };

	//���o�Q�[�W
	CVector3 m_musouPos = CVector3{ 0.0f,-304.0f,0.0f };

	//�v���C���[�|�C���^
	CVector3 m_position4 = CVector3{ 390.0f,192.0f,0.0f };
	CVector3 m_scale4 = CVector3::One() * 10.0f;

	//�f�B�t�@�[�g�J���[�B�����̈ȏ�|������A�J���[��ς���悤�ɂ������̂ł�����const�ł͐ݒ肵�Ȃ�
	CVector4 m_defeatColor = CVector4{ 1.0f,0.0f,0.0f,1.0f };//�t�H���g�̐F��ς���BR,G,B,A�̏��Ԃɕ���ł܂��B

	Player* m_player;
	Game*	m_game;
	Font m_font;			//�������p�t�H���g
	Font m_timeFont;		//�������ԗp�t�H���g
	//�v���C���[��HP��ۑ�
	int m_playerHP;
	//�O�t���[���̃v���C���[��HP��ۑ�
	int m_oldPlayerHP;
	//�v���C���[��񂪓����Ă��邩�H
	bool m_isPLInfo = false;
	float timer = 0.0f;

	float hyouzi = 0.0f;
	float timeVal = 0.75f;

	bool isTimeOver = false;
};