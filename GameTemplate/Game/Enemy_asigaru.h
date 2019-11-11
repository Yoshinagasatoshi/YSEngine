#pragma once
#include "Enemy.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "character/CharacterController.h"
#include "GameData.h"
/// <summary>
/// Enemyの派生クラス足軽
/// </summary>
class Enemy_asigaru : public Enemy
{
public:
	Enemy_asigaru();
	~Enemy_asigaru();
	//アプデ
	void Update();
	//描画
	void Draw();
	//動き
	void Move();
	void Turn();
	//戦闘距離に入った時に呼ばれるムーヴ
	void SentouMove();
	//idlePosの初期化
	void idlePosInit();
	//距離による判定処理関数
	void StateJudge();
	static Enemy& GetInstans()
	{
		static Enemy enemy;
		return enemy;
	}
	//足軽アニメ
	Animation m_asigaruAnime;
	//色んな足軽アニメを格納している配列
	AnimationClip m_asigaruAnimeClip[3];
	struct IdlePos
	{
		//周りの格納場所
		CVector3 idlePos;
		//格納場所が使われているか
		bool m_isUse = false;
		//初期化が行われたか？
		bool m_Syokika = false;
	};
	//配列数は待機場所の数。いまは４
	static IdlePos m_idlePos[5];
	CVector3 m_moveSpeed = CVector3::Zero();
	CVector3 moveV = CVector3::Zero();
	int i = 0;
	void SetGameDataInfo(GameData* gd)
	{
		m_gameData = gd;
	}
private:
	//キャラクターのコリジョン初期化
	void CharaconInit();
	
	//キャラの状態の種類
	enum AsigaruState {
		Asigaru_totugeki = 0,
		Asigaru_tikazuki = 1,
		Asigaru_sentou = 2
	};
	//足軽の初期ステートは？
	AsigaruState m_asigaruState = Asigaru_totugeki;
	//足軽の前にやっていたステート,アニメを流すかの判定に使用
	AsigaruState m_oldState = Asigaru_sentou;
	CharacterController m_characon;
	bool m_characonState = false;
	CVector3 m_forward = CVector3::AxisZ();
	double PI = 3.14159265358979323846f;
	//回るぞ
	float kaiten = 0.0f;
	GameData* m_gameData = nullptr;
};

