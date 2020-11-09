#include "stdafx.h"
#include "bom.h"
#include "gameObject/ysGameObjectManager.h"
#include "Fade.h"
#include "InGameSoundDirector.h"
const float BOM_RADIUS = 30.0f;			//ゴーストの当たり判定の大きさ用
const float BOM_GRAVITY_ACC = -1000.0f;	//爆弾の重力加速度。
const float BOM_VELOCITY_HORIZON = 200;	//爆弾の水平方向の移動速度。
const float BOM_SCALE = 3.0f;			//爆弾の大きさをいじれる。

const float BOM_Displacement = 0.5f;	//ボムの変位に使う数値

const float BOM_HIT_AREA = 150.0f;		//ボムがプレイヤー当たる範囲
const float BOM_LANDING_AREA = 100.0f;	//ボムが地面に着地したと認識する範囲

Bom::Bom()
{
	//ボム初期設定
	m_skinModel.Init(L"Assets/modelData/bom.cmo");
	m_skinModel.SetShadowCaster(false);
	m_ghostObject.CreateSphere(
		m_position,
		m_rotation,
		BOM_RADIUS
	);
}

Bom::~Bom()
{
	g_goMgr.DeleteGOObject(this);
}

void Bom::Update()
{
	//フェード状態(シーンの切り替えをしている状態)じゃなければ
	//爆弾としての処理をする
	if (Fade::Getinstance().IsFade()) {
		if (!m_isFirst) {
			FirstSet();//最初のボムの状態を作る

			//CSoundSource* se = new CSoundSource;
			//se->Init(L"Assets/sound/fuse.wav");
			//se->Play(false);
			//se->SetVolume(0.1f);

			//InGameSoundDirector::GetInstans().RingSE_Fuse();
		}

		//if (g_goMgr) {
		HitThebom();
		Finalbom();
		//重力加速度を加算
		m_bomVelocity.y += BOM_GRAVITY_ACC * GameTime().GetFrameDeltaTime();

		m_position += m_bomVelocity * GameTime().GetFrameDeltaTime();

		m_skinModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		//ボムの判定にぶれがあるため上方向に修正
		//m_bomGhostPos = m_position;
		//m_bomGhostPos.y += B_radius * 0.5f;
		//m_ghostObject.SetPosition(m_position);
	//}
	//else {
	//	DeleteGO(this);
	//}
	}
	//シーンの切り替えをしている時にはもう
	//プレイヤー等のインスタンスを参照したりする処理をしてほしくないので
	//ボムごとデリートする
	else {
		DeleteGO(this);
	}
}

void Bom::Draw()
{
	//モデルの描画
	m_skinModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

//一番最初にボムの設定をする。
//最初はプレイヤーに直接投げるのではなく、その中継点を目指す軌道をする。
void Bom::FirstSet()
{
	CVector3 vec = m_inpactPoint - m_position;
	vec.y = 0.0f;//Yの成分を除去
	vec.Normalize();
	vec *= BOM_VELOCITY_HORIZON;
	
	CVector3 distans = m_inpactPoint - m_position;
	distans.y = 0.0f;	//Yの成分を除去
	float a = BOM_GRAVITY_ACC * BOM_Displacement;
	float x =  m_inpactPoint.y - m_position.y;
	float t = distans.Length() / vec.Length();
	float Vo = (x - a * t * t) / t;
	vec.y = Vo;

	m_bomVelocity = vec;

	m_rotation = CQuaternion::Identity();//球体なので向いている方向は気にしない
	m_scale = CVector3::One() * BOM_SCALE; //爆弾モデルが小さかったので、3倍の大きさにした。
	m_isFirst = true;
}

void Bom::Finalbom()
{
	auto I_diff = m_inpactPoint - m_position;
	//着地処理。プレイヤーに当たっていない時の処理
	if (I_diff.Length() < BOM_LANDING_AREA) {
		//CSoundSource* se = new CSoundSource;
		//se->Init(L"Assets/sound/destruction.wav");
		//se->Play(false);
		//se->SetVolume(0.1f);
		g_Effect.m_sampleEffect = Effekseer::Effect::Create(g_Effect.m_effekseerManager, (const EFK_CHAR*)L"Assets/effect/bom.efk");
		//エフェクトを再生する。
		g_Effect.m_playEffectHandle = g_Effect.m_effekseerManager->Play(g_Effect.m_sampleEffect, m_position.x,m_position.y,m_position.z);

		DeleteGO(this);
	}
	Effekseer::Matrix44 efCameraMat;
	g_camera3D.GetViewMatrix().CopyTo(efCameraMat);
	Effekseer::Matrix44 efProjMat;
	g_camera3D.GetProjectionMatrix().CopyTo(efProjMat);
	g_Effect.m_effekseerRenderer->SetCameraMatrix(efCameraMat);
	g_Effect.m_effekseerRenderer->SetProjectionMatrix(efProjMat);
	g_Effect.m_effekseerManager->Update();
}

void Bom::HitThebom()
{
	//プレイヤーにボムが当たった時の処理
	if (!m_player->GetPlayerDead()) {
		if (m_player == nullptr) {
			DeleteGO(this);
		}
		auto P_diff = m_player->GetPosition() - m_position;
		if (P_diff.Length() < BOM_HIT_AREA)
		{
			m_player->PlayerDamage();

			//InGameSoundDirector::GetInstans().RingSE_Destruct();

			//CSoundSource* se = new CSoundSource;
			//se->Init(L"Assets/sound/destruction.wav");
			//se->Play(false);
			//se->SetVolume(0.1f);

			g_Effect.m_sampleEffect = Effekseer::Effect::Create(g_Effect.m_effekseerManager, (const EFK_CHAR*)L"Assets/effect/bom.efk");
			//エフェクトを再生する。
			g_Effect.m_playEffectHandle = g_Effect.m_effekseerManager->Play(g_Effect.m_sampleEffect, m_position.x, m_position.y, m_position.z);

			DeleteGO(this);
		}
		Effekseer::Matrix44 efCameraMat;
		g_camera3D.GetViewMatrix().CopyTo(efCameraMat);
		Effekseer::Matrix44 efProjMat;
		g_camera3D.GetProjectionMatrix().CopyTo(efProjMat);
		g_Effect.m_effekseerRenderer->SetCameraMatrix(efCameraMat);
		g_Effect.m_effekseerRenderer->SetProjectionMatrix(efProjMat);
		g_Effect.m_effekseerManager->Update();
	}
	else {
		DeleteGO(this);
	}
}