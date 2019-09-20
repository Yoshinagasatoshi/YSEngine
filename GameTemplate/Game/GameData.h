#pragma once
/// <summary>
/// ゲームデータクラス
/// <remarks>
/// ゲームデータのリスト等保存したほうが良いものを保存するクラス
/// </remarks>
/// </summary>
class Player;
class GameData
{
public:
	static GameData& GetInstans()
	{
		static GameData gamedata;
		return gamedata;
	}
	void SetPlayerPosition(const CVector3& Ppos)
	{
		m_playerPosition = Ppos;
	}
	CVector3 GetPlayerposition()
	{
		return m_playerPosition;
	}
private:
	CVector3 m_playerPosition;
};

