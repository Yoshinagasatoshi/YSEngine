#pragma once
#include "graphics/Sprite.h"
#include "gameObject/ysGameObject.h"
//UIクラス。体力とか無双ゲージとかつくるぞ
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
	Sprite m_sprite2;

	Sprite m_mapSprite;

	//sprite1 体力ゲージのデータ
	CVector3 m_position = CVector3{0.0f,-280.0f,0.0f};
	CQuaternion m_rotation = CQuaternion::Identity();
	CVector3 m_scale = CVector3::One();
	CVector2 m_pivot = { 0.0f,0.5f };

	//sprite2
	CVector3 m_position2 = CVector3{ -60.0f,-280.0f,0.0f };
	CQuaternion m_rotation2 = CQuaternion::Identity();
	CVector3 m_scale2 = CVector3::One();
	CVector2 m_pivot2 = { 0.5f,0.5f };

	//マップ
	CVector3 m_position3 = CVector3{ 200.0f,280.0f,0.0f };
	CQuaternion m_rotation3 = CQuaternion::Identity();
	CVector3 m_scale3 = CVector3::One();
	CVector2 m_pivot3 = { 0.5f,0.5f };

	Player* m_player;
	//プレイヤーのHPを保存
	int m_playerHP;
	//前フレームのプレイヤーのHPを保存
	int m_oldPlayerHP;
	//プレイヤー情報が入っているか？
	bool m_isPLInfo = false;
};