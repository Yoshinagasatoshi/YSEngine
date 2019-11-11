#pragma once
#include "character/CharacterController.h"
#include "gameObject/ysGameObject.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
//プレイヤークラス、無双武将にあたるクラス
class Player : public IGameObject
{
public:
	Player();
	~Player();
	//毎回の更新処理
	void Update();
	//毎度の描画処理
	void Draw();
	//恒例の移動処理
	void Move();
	//常設の回転処理
	void Turn();
	//キャラクターのコリジョン初期化
	void CharaconInit();
	CVector3& GetPosition()
	{
		return m_position;
	}
	//void SetEnemy(Enemy* enemy)
	//{
	//	m_enemy = enemy;
	//}
	//足軽アニメ
	Animation m_busyoAnime;
	//色んな武将アニメを格納している配列
	enum busyoBASICAnimeClip {
		animClip_idle = 0,
		animClip_ATK1,
		animClip_ATK2,
		animClip_ATK3,
		animClip_ATK4,
		animClip_ATK5,
		animClip_num
	};
	AnimationClip m_busyoAnimeClip[animClip_num];
private:
	void AttackMove();									//無双の攻撃の処理を書く
	int m_animStep = 0;									//アニメーションがどの段階か
	int m_oldAnimStep= 0;								
	int m_playTimer = 0;								//アニメが流されてどれくらい時間がたっているか。
	SkinModel m_playerModel;							//スキンモデル。
	CVector3 m_position = CVector3::Zero();				//プレイヤーの位置
	CQuaternion m_rotation = CQuaternion::Identity();   //プレイヤーの回転
	CVector3 m_scale = CVector3::One();					//プレイヤーの大きさ
	CVector3 m_moveSpeed = CVector3::Zero();			//プレイヤーの移動速度
	CharacterController m_characon;						//キャラクターコントローラー
	bool m_Jumpfrag = false;							//キャラはジャンプしているか？
	//Enemy* m_enemy = nullptr;
};

