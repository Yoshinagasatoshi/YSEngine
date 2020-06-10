#pragma once
/// <summary>
/// 武器クラス
/// クナイや爆弾などの飛び道具系の武器の挙動の親になるクラス
/// 武将や足軽が持っている武器は継承しないこと。
/// </summary>
class Wepon
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
protected:
	SkinModel m_kunaiModel;							//スキンモデル。
};

