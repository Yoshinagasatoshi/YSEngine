#pragma once
#include "gameObject/ysGameObject.h"
/// <summary>
/// 突発的に作ってしまったクラス
/// 敵とプレイヤーの距離を測って一番近い距離を渡す
/// 実質プレイヤークラス
///	変なクラスだほんとに…
/// </summary>
class Enemy;
class Player;
class Player_target : public IGameObject
{
public:
	//プレイヤーのデータを入れる
	void SetPlayerInfo(Player* pl)
	{
		m_player = pl;
	}
	//敵のデータを入れる
	void SetEnemyInfo(Enemy* en)
	{
		m_enemy = en;
	}
	//距離を返す
	CVector3 GetDistans()
	{
		return m_distans;
	}
	//補正スイッチを確認
	bool GetHosei()
	{
		return m_isHosei;
	}
	//補正スイッチをオフに
	void Hoseioff()
	{
		m_isHosei = false;
	}
	void HoseiJujge();
private:
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	CVector3 m_distans = CVector3::Zero();
	bool m_isHosei = false;
	//std::list<Enemy*> m_enemyList;
	int i = 0;//テストコード
};

