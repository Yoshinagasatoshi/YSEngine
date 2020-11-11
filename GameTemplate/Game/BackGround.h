#pragma once
#include "gameObject/ysGameObject.h"
#include "physics/PhysicsStaticObject.h"
class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();
	void Update();
	void Draw();
private:
	SkinModel m_model;										//スキンモデル。
	CVector3 m_position = CVector3{ 0.0f,-100.0f,0.0f };	//背景の位置
	CQuaternion m_rotation = CQuaternion::Identity();		//背景の回転角度
	CVector3 m_scale = CVector3::One();						//背景の大きさ
	PhysicsStaticObject m_physicsStaticObject;				//静的オブジェクト。メッシュを設定する
	ID3D11ShaderResourceView* m_earthNormalMapSRV;			//!<土法線マップのSRV。
	ID3D11ShaderResourceView* m_eatthSpecularMapSRV;		//!<土スペキュラマップのSRV。
	ID3D11ShaderResourceView* m_grassNormalMapSRV;			//!<草法線マップのSRV。
	ID3D11ShaderResourceView* m_grassSpecularMapSRV;		//!<草スペキュラマップのSRV。
};

