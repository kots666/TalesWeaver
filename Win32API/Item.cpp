#include "stdafx.h"
#include "Item.h"

CItem::CItem(int ID, int count) : m_ID(ID), m_count(count)
{
	Init();
}

CItem::~CItem()
{
}

void CItem::Init()
{
	m_atk = 0;
	m_money = ITEM::COST[m_ID];

	switch (m_ID)
	{
	case 0: m_key = __T("HP 회복 포션"); break;
	case 1: m_key = __T("MP 회복 포션"); break;
	case 2: m_key = __T("SP 회복 포션"); break;
	case 3: m_key = __T("고급 갑옷"); break;
	case 4: m_key = __T("하급 갑옷"); break;
	case 5: m_key = __T("고급 검"); break;
	case 6: m_key = __T("하급 검"); break;
	case 7: m_key = __T("고급 신발"); break;
	case 8: m_key = __T("하급 신발"); break;
	case 9: m_key = __T("고급 반지"); break;
	case 10: m_key = __T("하급 반지"); break;
	case 11: m_key = __T("고급 모자"); break;
	case 12: m_key = __T("하급 모자"); break;
	case 13: m_key = __T("수상한 물건"); break;
	}

	UpdateRect();
}

int CItem::Update()
{
	if (m_isDead) return DEAD_EVENT;

	return NO_EVENT;
}

void CItem::LateUpdate()
{
}

void CItem::Render(HDC hDC)
{
	HDC memDC = CBitmapManager::GetInstance()->GetDC(__T("Items"));

	TransparentBlt(hDC, m_rect.left, m_rect.top, 24, 24, memDC, m_ID * 24, 0, 24, 24, RGB(255, 0, 255));
}

void CItem::Release()
{
}
