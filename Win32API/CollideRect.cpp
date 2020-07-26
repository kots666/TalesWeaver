#include "stdafx.h"
#include "CollideRect.h"
#include "Effect.h"

CCollideRect::CCollideRect(float x, float y, int cx, int cy, int damage, DWORD lifeTime, bool isPlayer)
	: m_spawnTime(GetTickCount()), m_lifeTime(lifeTime), m_isHit(false), m_isPlayer(isPlayer)
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
		if (!m_isHit && m_isPlayer)
		{
			CComboManager::ResetCombo();

			CObj* player = CObjManager::GetInstance()->GetPlayer();
			if (nullptr == player) return;

			INFO playerInfo = player->GetInfo();

			float x = playerInfo.xPos;
			float y = playerInfo.yPos - 80;
			int cx = 46;
			int cy = 20;

			CObj* miss = new CEffect(x, y, cx, cy, __T("Miss"), 0, 1, 500, RGB(255, 0, 255), true);
			CObjManager::GetInstance()->AddObject(miss, OBJ::EFFECT);

			CSoundManager::GetInstance()->PlayOverlapSound(__T("Miss.mp3"), CSoundManager::PLAYER);
		}
		m_isDead = true;
	}
	UpdateRect();
}

void CCollideRect::Render(HDC hDC)
{
}

void CCollideRect::Release()
{
}
