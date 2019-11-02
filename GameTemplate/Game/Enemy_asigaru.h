#pragma once
#include "Enemy.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "character/CharacterController.h"

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
	void Move() override;
	//戦闘距離に入った時に呼ばれるムーヴ
	void SentouMove();
	//idlePosの初期化
	void idlePosInit();
	//距離による判定処理関数
	void LenghtJudge();
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
	CVector3 m_moveSpeed;
	CVector3 moveV = CVector3::Zero();
	int i = 0;
private:
	//キャラクターのコリジョン初期化
	void CharaconInit();
	//キャラのステート
	enum AsigaruState {
		Asigaru_idle = 0, 
		Asigaru_totugeki,
		Asigaru_sentou
	};
	AsigaruState m_state = Asigaru_idle;
	CharacterController m_characon;
	bool s = false;
};

