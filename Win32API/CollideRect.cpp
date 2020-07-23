#include "stdafx.h"
#include "CollideRect.h"


CCollideRect::CCollideRect(float x, float y, int cx, int cy, int damage, DWORD lifeTime)
	: m_spawnTime(GetTickCount()), m_lifeTime(lifeTime), m_isHit(false)
{
	m_info.xPos = x;
	m_info.yPos = y;
	m_info.xSize = cx;
	m_info.ySize = cy;
	m_atk = damage;

	UpdateRect();
}


CCollideRect::~CCollideRect()
{
}

void CCollideRect::Init()
{
}

int CCollideRect::Update()
{
	if (m_isDead) return DEAD_EVENT;

	return NO_EVENT;
}

void CCollideRect::LateUpdate()
{
	if (m_spawnTime + m_lifeTime <= GetTickCount())
	{
		if (!m_isHit) CComboManager::ResetCombo();
		m_isDead = true;
	}
	UpdateRect();
}

void CCollideRect::Render(HDC hDC)
{
	/*
	HDC memDC = CBitmapManager::GetInstance()->GetDC(__T("Tile_Normal"));

	StretchBlt(hDC, m_rect.left - CCamera::GetX(), m_rect.top - CCamera::GetY(), m_info.xSize, m_info.ySize, memDC, 0, 0, 30, 30, SRCCOPY);
	*/
}

void CCollideRect::Release()
{
}
