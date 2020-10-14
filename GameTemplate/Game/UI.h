#pragma once
#include "graphics/Sprite.h"
#include "gameObject/ysGameObject.h"
//UIクラス。体力バー等の画像を張る
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
	Sprite m_lifeGauge;
	Sprite m_lifeGaugeura;

	Sprite m_face;

	Sprite m_mapSprite;

	Sprite m_playerPointer;
	Sprite m_playerPointer_yazirushi;


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
	CVector3 m_position3 = CVector3{ 380.0f,240.0f,0.0f };
	CQuaternion m_rotation3 = CQuaternion::Identity();
	CVector3 m_scale3 = CVector3::One();
	CVector2 m_pivot3 = { 0.5f,0.5f };

	//プレイヤーポインタ
	CVector3 m_position4 = CVector3{ 390.0f,192.0f,0.0f };
	CVector3 m_scale4 = CVector3::One() * 10.0f;

	Player* m_player;
	//プレイヤーのHPを保存
	int m_playerHP;
	//前フレームのプレイヤーのHPを保存
	int m_oldPlayerHP;
	//プレイヤー情報が入っているか？
	bool m_isPLInfo = false;
};