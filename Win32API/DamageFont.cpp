#include "stdafx.h"
#include "DamageFont.h"

CDamageFont::CDamageFont(float x, float y, int id) : m_ID(id), m_spawnTime(GetTickCount())
{
	m_info.xPos = x;
	m_info.yPos = y;

	Init();
}

CDamageFont::~CDamageFont()
{
	Release();
}

void CDamageFont::Init()
{
	m_info.xSize = 23;
	m_info.ySize = 30;

	UpdateRect();
}

int CDamageFont::Update()
{
	if (m_isDead) return DEAD_EVENT;

	--m_info.yPos;

	return NO_EVENT;
}

void CDamageFont::LateUpdate()
{
	if (m_spawnTime + 1000 < GetTickCount()) m_isDead = true;
}

void CDamageFont::Render(HDC hDC)
{
	HDC memDC = CBitmapManager::GetInstance()->GetDC(__T("Number"));

	TransparentBlt(hDC, m_info.xPos - CCamera::GetX(), m_info.yPos - CCamera::GetY(), m_info.xSize, m_info.ySize, memDC, m_ID * m_info.xSize, 0, m_info.xSize, m_info.ySize, RGB(255, 0, 255));
}

void CDamageFont::Release()
{
}
