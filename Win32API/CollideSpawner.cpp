#include "stdafx.h"
#include "CollideSpawner.h"
#include "CollideRect.h"

CCollideSpawner::CCollideSpawner(float x, float y, int cx, int cy, int damage, int spawnCount, DWORD gapTime, OBJ::TAG type, bool isPlayer)
	: m_x(x), m_y(y), m_cx(cx), m_cy(cy), m_damage(damage),
	m_spawnCount(spawnCount), m_gapTime(gapTime), m_type(type), m_spawnTime(GetTickCount()),
	m_isPlayer(isPlayer)
{
	Init();
}

CCollideSpawner::~CCollideSpawner()
{
}

void CCollideSpawner::Init()
{
	m_spawnTime -= m_gapTime;
}

int CCollideSpawner::Update()
{
	if (m_isDead) return DEAD_EVENT;

	if (m_spawnTime + m_gapTime <= GetTickCount())
	{
		CObj* newCollide = new CCollideRect(m_x, m_y, m_cx, m_cy, m_damage, 0, m_isPlayer);
		CObjManager::GetInstance()->AddObject(newCollide, m_type);

		--m_spawnCount;
		m_spawnTime = GetTickCount();
	}

	return 0;
}

void CCollideSpawner::LateUpdate()
{
	if (m_spawnCount < 1) m_isDead = true;
}

void CCollideSpawner::Render(HDC hDC)
{
}

void CCollideSpawner::Release()
{
}
