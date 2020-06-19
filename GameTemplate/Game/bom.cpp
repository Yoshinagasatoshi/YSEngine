#include "stdafx.h"
#include "bom.h"
#include "gameObject/ysGameObjectManager.h"

const float B_radius = 30.0f;			//ゴーストの当たり判定の大きさ用
const float BOM_GRAVITY_ACC = -400.0f;	//爆弾の重力加速度。
bom::bom()
{
	m_skinModel.Init(L"Assets/modelData/bom.cmo");
	m_ghostObject.CreateSphere(
		m_position,
		m_rotation,
		B_radius
	);
}

bom::~bom()
{
	g_goMgr.DeleteGOObject(this);
}

void bom::Update()
{
	if (!m_isFirst) {
		FirstSet();//所見の大きさとか設定
	}
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
}

void bom::Draw()
{
	//モデルの描画
	m_skinModel.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

//一番最初にボムの設定をする。
//最初はプレイヤーに直接投げるのではなく、その中継点を目指す軌道をする。
void bom::FirstSet()
{
	CVector3 vec = m_inpactPoint - m_position;
	vec.y = 0.0f;
	vec.Normalize();
	vec *= 400.0f;
	
	CVector3 distans = m_inpactPoint - m_position;
	distans.y = 0.0f;
	float a = BOM_GRAVITY_ACC * 0.5f;
	float x =  m_inpactPoint.y - m_position.y;
	float t = distans.Length() / vec.Length();
	float Vo = (x - a * t * t) / t;
	vec.y = Vo;

	m_bomVelocity = vec;

	m_rotation = CQuaternion::Identity();//球体かつデフォルメされているので、どの方向が正面かわからんから気にしなくていい
	m_scale = CVector3::One() * 3.0f; //モデルが小さかったので、3倍の大きさにした。
	m_isFirst = true;
}

void bom::Finalbom()
{
	auto I_diff = m_inpactPoint - m_position;
	//着地処理。プレイヤーに当たっていない時の処理
	if (I_diff.Length() < 100.0f) {
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

void bom::HitThebom()
{
	//エラーがよく起こる場所
	//プレイヤーが死んだ後に、投げられた状態のボムが死んだプレイヤーの距離を測っているからエラーが起こっている？
	if (!m_player->GetPlayerDead()) {
		auto P_diff = m_player->GetPosition() - m_position;
		if (P_diff.Length() < 150.0f)
		{
			m_player->PlayerDamage();
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