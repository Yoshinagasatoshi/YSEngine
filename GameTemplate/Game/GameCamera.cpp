#include "stdafx.h"
#include "GameCamera.h"
GameCamera::GameCamera()
{
	
}
GameCamera::~GameCamera()
{

}
void GameCamera::Update()
{
	CVector3 target = m_player->GetPosition();
	CVector3 toPos{ 0.0f, 100.0f, 300.0f };
	m_position = m_player->GetPosition() + toPos;
	g_camera3D.SetTarget(target);
	g_camera3D.SetPosition(m_position);
}
void GameCamera::Draw()
{

}