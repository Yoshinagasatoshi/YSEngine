#include "stdafx.h"
#include "Wepon_ghost.h"
#include "gameObject/ysGameObjectManager.h"
Wepon_ghost::Wepon_ghost()
{
	
}

Wepon_ghost::~Wepon_ghost()
{
	m_ghostObject.Release();
}

bool Wepon_ghost::Start()
{
	return true;
}

void Wepon_ghost::Update()
{
	GhostInit();
}

void Wepon_ghost::GhostInit()
{
	if (m_hanteifin) {
		DeleteGO(this);
		timer = 0;
	}
	if (!m_hanteifin) {
		m_ghostObject.CreateBox(
			m_position,
			m_rotation,
			m_scale * 100.0f
		);
		m_hanteifin = true;
	}
	
}