#pragma once
#include "Enemy.h"
#include "character/CharacterController.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
class bom;
class Enemy_Bomber : public Enemy
{
public:
	Enemy_Bomber();
	~Enemy_Bomber();

	void Draw();
	void Update();
	//ステート管理
	void Statekanri();
	//アニメーション管理
	void Animekanri();
	//回転
	void Turn();
	//動き
	void EscapeMove();
	//攻撃。
	void attackMove();
	//死ぬ。
	void DeathMove();
	//Moveのディレクター。どのムーブを繰り出すか決める。
	void Move();
	//キャラコーンを作成
	void CharaconInit();
	//キャラの状態の種類
	enum AsigaruState {
		Asigaru_attack,//このアタックは投げるアニメーション
		Asigaru_zyosou,
		Asigaru_damage,
		Asigaru_dead,
		Asigaru_escape,
		Asigaru_Idle,
		Asigaru_anim_num
	};
	//爆弾兵アニメ。
	Animation m_bomberAnime;
	//アニメを格納している配列
	AnimationClip m_bomAnimeClip[Asigaru_anim_num];
private:
	CharacterController m_characon;//キャラコン
	AsigaruState m_state = Asigaru_zyosou;//最初の状態は助走しているモード
	bom* m_bom;//ボムを出すため付随
	CVector3 m_moveSpeed = CVector3::Zero();//移動速度
	bool m_isCharaconUse = false;//キャラコン入ってる？
	bool m_isDeadfrag = false;	//bomberが死んだかどうか
};