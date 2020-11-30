#pragma once
#include "gameObject/ysGameObject.h"
/// <summary>
/// 米俵クラス
/// 体力を回復させるためのアイテムを出すクラス
/// プレイヤーの攻撃によって破壊される
/// つまり、プレイヤーが攻撃したという信号を受け取らなければいけない
/// </summary>
class Player;
class Onigiri;
class Komedawara : public IGameObject
{
public:
	Komedawara();
	~Komedawara();
	/// <summary>
	/// 現在地の設定
	/// </summary>
	/// <param name="pos"></param><returns></returns>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="qrot"></param>
	/// <returns></returns>
	void SetRot(CQuaternion qrot)
	{
		m_rot = qrot;
	}
	/// <summary>
	/// 大きさの設定
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	void SetScale(CVector3 scale)
	{
		m_scale = scale;
	}
	void SetPlayerInfo(Player* PL)
	{
		m_player = PL;
	}
private:
	void Update();
	void Draw();

	CVector3 m_position = CVector3::Zero();	//モデル位置
	CQuaternion m_rot = CQuaternion::Identity();//モデル回転
	CVector3 m_scale = CVector3::One();		//モデルの大きさ

	Player*	m_player;				//プレイヤーのポインタ
	Onigiri* m_onigiri;				//回復アイテムのポインタ
	SkinModel m_komedawaraModel;//米俵のモデル
};

