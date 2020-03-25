#pragma once
#include "graphics/Sprite.h"
#include "gameObject/ysGameObject.h"
//UI�N���X�B�̗͂Ƃ����o�Q�[�W�Ƃ����邼
class Player;
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
private:
	Sprite m_sprite;
	CVector3 m_position = CVector3{60.0f,-200.0f,0.0f};
	CQuaternion m_rotation = CQuaternion::Identity();
	CVector3 m_scale = CVector3::One();
	CVector2 m_pivot = { 0.5f,0.5f };
	Player* m_player;
	//�v���C���[��HP��ۑ�
	int m_playerHP;
	//�O�t���[���̃v���C���[��HP��ۑ�
	int m_oldPlayerHP;
	//�v���C���[��񂪓����Ă��邩�H
	bool m_isPLInfo = false;
};