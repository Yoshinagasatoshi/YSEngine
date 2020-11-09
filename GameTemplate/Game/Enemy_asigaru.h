#pragma once
#include "Enemy.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "character/CharacterController.h"
#include "math/Matrix.h"
#include "physics/PhysicsGhostObject.h"
#include "graphics/Skeleton.h"
/// <summary>
/// Enemyの派生クラス足軽
/// </summary>
class GameData;
class Wepon_ghost;
class InGameSoundDirector;
class Enemy_asigaru : public Enemy
{
public:
	Enemy_asigaru();
	~Enemy_asigaru();
	//アプデ
	void Update();
	//描画
	void Draw();
	//倒されていない時に呼ぶ関数
	void Move();
	void Turn();
	//倒されたときに呼ぶ関数
	void DeadMove();
	//idlePosの初期化
	void idlePosInit();
	//距離による判定処理関数
	void StateJudge();
	//ゴーストのInit
	//void ghostInit();

	//ふっとばし量を受け取る。
	void SetDamage(float recive)
	{
		m_recivePower = recive;
	}

	static Enemy_asigaru& GetInstans()
	{
		static Enemy_asigaru enemy;
		return enemy;
	}
	
	//PhysicsGhostObject* GetGhostObject(){
	//	return &m_ghostObject;
	//}
	//キャラの状態の種類
	enum AsigaruState {
		Asigaru_totugeki = 0,
		Asigaru_tikazuki,
		Asigaru_sentou,
		Asigaru_attack,
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
	//配列数は待機場所の数。いまは5
	static const int m_Waiting_Point = 5;			//待機場所の数
	static IdlePos M_IDLEPOS[m_Waiting_Point];
	CVector3 m_moveSpeed = CVector3::Zero();
	CVector3 moveV = CVector3::Zero();
	int i = 0;
private:
	//攻撃時間が何秒か抽選する関数
	//計測時間も初期化する
	float AttackframeNum()
	{
		m_kougekiframenum = 100.0f + rand() % 500;
		return m_kougekiframenum;
	}
	//アニメーションイベントを呼び出す
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//キャラクターのコリジョン初期化
	void CharaconInit();
	//当たり判定を出す変数
	//PhysicsGhostObject m_ghostObject;
	//足軽の状態管理。
	AsigaruState m_asigaruState = Asigaru_totugeki;
	//足軽の前にやっていたステート,アニメを流すかの判定に使用
	AsigaruState m_oldState = Asigaru_sentou;

	//キャラクターコントローラ。コリジョンを簡単につけるもの
	CharacterController m_characon;

	//キャラコンを入れる。
	bool m_characonConUse = false;
	//試しに書いているコード
	bool m_isDeadfrag = false;
	//攻撃中のフラグ
	bool m_isAttack = false;
	//ストッパー。一回しか呼ばれたくないので
	bool m_deadMoveStopper = false;
	//プレイヤーが死んでいる状態かどうかを受け取る変数。
	//関数で受け取るので初期化してません
	bool m_player_isdead;

	//自分の回転
	//float m_modelRotate = 0.0f;
	//計測時間
	float m_frameTimer = 0.0f;
	//攻撃時間。この時間を超えると攻撃フラグがたつ
	float m_kougekiframenum = 99.9f;
	//消えるまでの時間
	float m_Deathtimer_f = 0.0f;
	//吹き飛ばし量を入れる箱
	float m_recivePower;
	//円周率。必要なためここに書いた
	double m_PI = 3.14159265358979323846f;
	//取り巻けなかったasigaruが使う変数
	bool m_torimaki = false;

	//前方向
	CVector3 m_forward = CVector3::AxisZ();

	//自分が出す武器のゴーストのポインタ。
	Wepon_ghost* m_en_Wepon;
	//スケルトン。骨の位置にghostをだすため。
	Skeleton* m_skeleton;
	//ゲームデータのポインタ。数値をここから抜粋していく。
	GameData* gamedata = nullptr;
	//SEがどれだけなっているかをカウントする。
	int m_seRingCount = 0;
	//待機待ちのseがどれだけあるかをカウントする。
	int m_seStockCount = 0;
	//鳴らしている？
	bool m_isRingSE = false;
	InGameSoundDirector* m_sd = nullptr;
	float m_timer = 10.0f;
	bool m_isFirstUpdate = true;
	bool m_CCount = false;
}; 

