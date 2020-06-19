#pragma once
#include "gameObject/ysGameObject.h"
/// <summary>
/// 武器クラス
/// クナイや爆弾などの飛び道具系の武器の挙動の親になるクラス
/// 武将や足軽が持っている武器は継承しないこと。
/// </summary>
class Enemy	;
class Wepon : public IGameObject
{
public:
	Wepon();
	~Wepon();
private:
	//一定時間たつと消える関数
	virtual void DispTime();
	//武器ごとの攻撃判定を出す関数。
	virtual void WeponATKjuge();
	//アプデ
	virtual void Update();

	CVector3 SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	CVector3 SetScale(CVector3 scale)
	{
		m_scale = scale;
	}
	CQuaternion SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
protected:
	SkinModel m_skinModel;							//スキンモデル。
	CVector3 m_position = CVector3::Zero();
	CVector3 m_scale = CVector3::One();
	CQuaternion m_rotation = CQuaternion::Identity();
};

