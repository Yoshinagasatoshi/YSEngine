#pragma once
#include "gameObject/ysGameObject.h"
/// <summary>
/// おにぎりクラス
/// 米俵クラスが攻撃によって破壊されたら出る
/// 回復アイテムのクラス
/// </summary>
class Player;
class UI;
class Onigiri : public IGameObject
{
public:
	Onigiri();
	~Onigiri();

	void GetAitem();			//アイテムを取得したときに呼ばれる
	void SetPlayer(Player* PL)			//プレイヤー情報をセット
	{
		m_player = PL;
	}
	void SetPosition(CVector3 pos)//場所をセット
	{
		m_position = pos;
	}
private:
	void Update();
	void Draw();
	SkinModel m_onigiriModel;	//おにぎりのモデル。回復アイテムとして使用

	CVector3 m_position;						//おにぎりモデルの位置
	CQuaternion m_rot = CQuaternion::Identity();//おにぎりモデルの回転
	CVector3 m_scale = CVector3::One();			//おにぎりモデルの大きさ

	float m_rotValue = 0.0f;					//回転量の値
	Player* m_player;							//プレイヤーのポインタ
};

