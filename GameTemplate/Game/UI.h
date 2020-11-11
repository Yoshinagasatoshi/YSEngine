#pragma once
#include "graphics/Sprite.h"
#include "gameObject/ysGameObject.h"
#include "Font.h"
//UIクラス。体力バー等の画像を張る
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
	Sprite m_lifeGauge;					//体力ゲージ
	Sprite m_lifeGaugeura;				//体力ゲージの裏側。体力ゲージが見えやすくするためのもの

	Sprite m_musouGauge;				//無双ゲージ
	Sprite m_musouGaugeura;				//無双ゲージの裏側。無双ゲージを見えやすくするもの

	Sprite m_face;						//武将アイコン

	Sprite m_mapSprite;					//ミニマップ画像

	Sprite m_playerPointer;				//ミニマップのプレイヤーアイコン
	//Sprite m_playerPointer_yazirushi;

	//sprite1 体力ゲージのデータ
	//画像位置、画像角度、画像幅、画像基点の順に設定
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

	//無双ゲージ
	CVector3 m_musouPos = CVector3{ 0.0f,-304.0f,0.0f };

	//プレイヤーポインタ
	CVector3 m_position4 = CVector3{ 390.0f,192.0f,0.0f };
	CVector3 m_scale4 = CVector3::One() * 10.0f;

	//ディファートカラー。○○体以上倒したら、カラーを変えるようにしたいのでこいつはconstでは設定しない
	CVector4 m_defeatColor = CVector4{ 1.0f,0.0f,0.0f,1.0f };//フォントの色を変える。R,G,B,Aの順番に並んでます。

	Player* m_player;		//プレイヤーのポインタ
	Game*	m_game;			//ゲームのポインタ
	Font m_font;			//討伐数用フォント
	Font m_timeFont;		//制限時間用フォント
	//プレイヤーのHPを保存
	int m_playerHP;
	//前フレームのプレイヤーのHPを保存
	int m_oldPlayerHP;
	//プレイヤー情報が入っているか？
	bool m_isPLInfo = false;	
	float timer = 0.0f;			//どれだけ制限時間がたったかを記録する

	float hyouzi = 0.0f;		//無双ゲージをどれだけ表示するか

	bool isTimeOver = false;	//時間切れになっているのか？
};