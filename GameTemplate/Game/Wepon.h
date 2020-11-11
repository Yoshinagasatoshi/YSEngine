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

	/// <summary>
	/// 場所の設定
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	CVector3 SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// 大きさの設定
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	CVector3 SetScale(CVector3 scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// 回転角度の設定
	/// </summary>
	/// <param name="rot"></param>
	/// <returns></returns>
	CQuaternion SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
protected:
	SkinModel m_skinModel;							//スキンモデル。
	CVector3 m_position = CVector3::Zero();			//武器位置
	CVector3 m_scale = CVector3::One();				//武器の大きさ
	CQuaternion m_rotation = CQuaternion::Identity();//武器角度
};

