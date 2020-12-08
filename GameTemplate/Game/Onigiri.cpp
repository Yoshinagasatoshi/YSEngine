#include "stdafx.h"
#include "Onigiri.h"
#include "gameObject/ysGameObjectManager.h"
#include "Player.h"
#include "InGameSoundDirector.h"
const CVector3 AXIS_Y = { 0.0f,1.0f,0.0f };	//Y軸
const float ADD_ROT = 5.0f;					//毎フレームどれだけ回転させるか
const float GET_ITEM_RANGE = 100.0f;			//アイテムをゲットしたといえる範囲
Onigiri::Onigiri()
{
	m_onigiriModel.Init(L"Assets/modelData/onigiri.cmo");
}


Onigiri::~Onigiri()
{
}

void Onigiri::Update()
{

	//回転角度を毎回足す
	m_rotValue += ADD_ROT;
	//出てきたらくるくる回る
	m_rot.SetRotationDeg(AXIS_Y,m_rotValue);
	//エフェクトで光らす　取ったら回復して消える
	m_onigiriModel.UpdateWorldMatrix(m_position, m_rot, m_scale);
	CVector3 Distance = m_position - m_player->GetPosition();

	//プレイヤーと近ければ取得扱いになる
	if (Distance.Length() < GET_ITEM_RANGE) {
		GetAitem();
		InGameSoundDirector::GetInstans().RingSE_HEAL();
		m_player->PLAYER_HEAL();
	}

}

void Onigiri::Draw()
{
	//おにぎりモデルの描画
	m_onigiriModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void Onigiri::GetAitem()
{
	//プレイヤーに回復を与えて消える
	g_goMgr.DeleteGOObject(this);
}