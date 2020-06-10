#include "stdafx.h"
#include "gameObject/ysGameObjectManager.h"
#include "Enemy_Bomber.h"

Enemy_Bomber::Enemy_Bomber()
{
	//ボム敵のモデルはasigaruのモデルに槍を取っ払ったものなので
	//倒れる時とかのアニメはasigaruのアニメを流用します。
	m_model.Init(L"Assets/modelData/asigaru_taicho.cmo");
	m_rotation = CQuaternion::Identity();
	m_scale = CVector3::One();
	m_bomAnimeClip[Asigaru_attack].Load(L"Assets/animData/asigaru_nage.tka");
	m_bomAnimeClip[Asigaru_damage].Load(L"Assets/animData/asigaru_damage.tka");
	m_bomAnimeClip[Asigaru_dead].Load(L"Assets/animData/asigaru_dead.tka");
	m_bomAnimeClip[Asigaru_zyosou].Load(L"Assets/animData/asigaru_zyosou.tka");
	m_bomAnimeClip[Asigaru_attack].SetLoopFlag(false);
	m_bomAnimeClip[Asigaru_damage].SetLoopFlag(false);
	m_bomAnimeClip[Asigaru_dead].SetLoopFlag(false);
	m_bomAnimeClip[Asigaru_zyosou].SetLoopFlag(true);
	//モデルとアニメデータを初期化
	m_bomberAnime.Init(
		m_model,
		m_bomAnimeClip,
		Asigaru_anim_num//テスト
	);
}

Enemy_Bomber::~Enemy_Bomber()
{
	g_goMgr.DeleteGOObject(this);
}

void Enemy_Bomber::Draw()
{
	//モデルの描画
	m_model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void Enemy_Bomber::Update()
{
	m_bomberAnime.Play(Asigaru_zyosou, 0.2f);
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_bomberAnime.Update(1.0f / 30.0f);
}