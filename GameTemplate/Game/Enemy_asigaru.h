#pragma once
#include "Enemy.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "character/CharacterController.h"
#include "math/Matrix.h"
#include "physics/PhysicsGhostObject.h"
/// <summary>
/// Enemyの派生クラス足軽
/// </summary>
class GameData;
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
	//死んだときのムーヴ
	void DeadMove();
	//idlePosの初期化
	void idlePosInit();
	//距離による判定処理関数
	void StateJudge();
	//ゴーストのInit
	void ghostInit();

	static Enemy& GetInstans()
	{
		static Enemy enemy;
		return enemy;
	}

	PhysicsGhostObject* GetGhostObject(){
		return &m_ghostObject;
	}
	//キャラの状態の種類
	enum AsigaruState {
		Asigaru_totugeki = 0,
		Asigaru_tikazuki,
		Asigaru_sentou,
		Asigaru_damage,
		Asigaru_dead,
		Asigaru_anim_num
	};
	//足軽アニメ
	Animation m_asigaruAnime;
	//色んな足軽アニメを格納している配列
	AnimationClip m_asigaruAnimeClip[Asigaru_anim_num];
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
private:
	//アニメーションイベントを呼び出すよ
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//キャラクターのコリジョン初期化
	void CharaconInit();
	//ghost!
	PhysicsGhostObject m_ghostObject;
	//足軽の初期ステートは？
	AsigaruState m_asigaruState = Asigaru_totugeki;
	//足軽の前にやっていたステート,アニメを流すかの判定に使用
	AsigaruState m_oldState = Asigaru_sentou;
	CharacterController m_characon;
	bool m_characonState = false;
	CVector3 m_forward = CVector3::AxisZ();
	double PI = 3.14159265358979323846f;
	//回るぞ
	//CMatrix a;
	float kaiten = 0.0f;
	GameData* gamedata = nullptr;

	//試しに書いているコード
	bool isdeadfrag = false;
};

