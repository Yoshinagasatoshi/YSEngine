#pragma once
#include "Enemy.h"
#include "character/CharacterController.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
class Enemy_Bomber : public Enemy
{
public:
	Enemy_Bomber();
	~Enemy_Bomber();

	void Draw();
	void Update();
	//キャラの状態の種類
	enum AsigaruState {
		Asigaru_attack,//このアタックは投げるアニメーション
		Asigaru_zyosou,
		Asigaru_damage,
		Asigaru_dead,
		Asigaru_anim_num
	};
	//爆弾兵アニメ。
	Animation m_bomberAnime;
	//アニメを格納している配列
	AnimationClip m_bomAnimeClip[Asigaru_anim_num];
private:
	//CharacterController m_characon;//キャラコン
};