#include "stdafx.h"
#include "InfoButton.h"
#include "Player.h"
#include "Item.h"

CInfoButton::CInfoButton(float x, float y, int cx, int cy, const TCHAR * key, int cx2, int cy2, const TCHAR * child)
	: m_key(key), m_childKey(child), m_childCX(cx2), m_childCY(cy2), m_isChildOpen(false)
{
	m_info.xPos = x;
	m_info.yPos = y;
	m_info.xSize = cx;
	m_info.ySize = cy;
	m_ID = 0;

	Init();
}

CInfoButton::~CInfoButton()
{
	Release();
}

void CInfoButton::Init()
{
	UpdateRect();
}

int CInfoButton::Update()
{
	if (m_isDead) return DEAD_EVENT;

	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	RECT rc = { m_info.xPos, m_info.yPos, m_info.xPos + m_info.xSize, m_info.yPos + m_info.ySize };

	if (PtInRect(&rc, pt))
	{
		if(!m_ID) CSoundManager::GetInstance()->PlayOverlapSound(__T("UI_Over.wav"), CSoundManager::UI);

		m_ID = 1;

		if (CKeyManager::GetInstance()->isKeyDown(KEY_LBUTTON))
		{
			CSoundManager::GetInstance()->PlayOverlapSound(__T("UI_Click.mp3"), CSoundManager::UI);
			m_isChildOpen ^= true;
			m_ID = 1;
		}
	}
	else m_ID = 0;

	InvenToEquip(pt);
	EquipToInven(pt);

	return NO_EVENT;
}

void CInfoButton::LateUpdate()
{
}

void CInfoButton::Render(HDC hDC)
{
	HDC memDC = CBitmapManager::GetInstance()->GetDC(m_key);

	int x = m_info.xPos;
	int y = m_info.yPos;
	int width = m_info.xSize;
	int height = m_info.ySize;
	DWORD color = RGB(255, 0, 255);

	TransparentBlt(hDC, x, y, width, height, memDC, m_ID * width, 0, width, height, color);

	if (m_isChildOpen)
	{
		HDC childDC = CBitmapManager::GetInstance()->GetDC(m_childKey);

		int childX = x - m_childCX;
		int childWidth = m_childCX;
		int childHeight = m_childCY;

		TransparentBlt(hDC, childX, y, childWidth, childHeight, childDC, 0, 0, childWidth, childHeight, color);

		if (!(lstrcmp(m_childKey, __T("Stat_Info")))) StatRender(hDC, childX, y);
		if (!(lstrcmp(m_childKey, __T("Equip_Info")))) EquipRender(hDC, childX, y);
		if (!(lstrcmp(m_childKey, __T("Inven_Info")))) InvenRender(hDC, childX, y);
	}
}

void CInfoButton::Release()
{
}

void CInfoButton::InvenToEquip(POINT& pt)
{
	if (!lstrcmp(m_childKey, __T("Inven_Info")) && m_isChildOpen)
	{
		int size = CInvenManager::GetInstance()->GetSize();
		for (int i = 0; i < 12; ++i)
		{
			if (i >= size) break;

			int x = m_info.xPos - m_childCX + 11;
			int y = m_info.yPos + 42 + (i * 25);

			RECT rc = { x, y, x + 24, y + 24 };

			if (PtInRect(&rc, pt))
			{
				if (CKeyManager::GetInstance()->isKeyDown(KEY_LBUTTON))
				{
					int ID = CInvenManager::GetInstance()->GetInventory()->at(i)->GetID();

					if (CEquipManager::GetInstance()->CanEquip(ID))
					{
						CEquipManager::GetInstance()->EquipItem(ID);
						CInvenManager::GetInstance()->DecreaseItem(ID);

						CSoundManager::GetInstance()->PlayOverlapSound(__T("UI_Click.mp3"), CSoundManager::UI);
					}
				}
			}
		}
	}
}

void CInfoButton::EquipToInven(POINT& pt)
{
	if (!lstrcmp(m_childKey, __T("Equip_Info")) && m_isChildOpen)
	{
		for (int i = 0; i < 5; ++i)
		{
			int x = m_info.xPos - m_childCX + 6;
			int y = m_info.yPos + 26 + (i * 31);

			RECT rc = { x, y, x + 24, y + 24 };

			if (PtInRect(&rc, pt))
			{
				if (CKeyManager::GetInstance()->isKeyDown(KEY_LBUTTON))
				{
					CEquipManager::GetInstance()->UnequipItem(i);
					CSoundManager::GetInstance()->PlayOverlapSound(__T("UI_Click.mp3"), CSoundManager::UI);
				}
			}
		}
	}
}

void CInfoButton::StatRender(HDC hDC, float x, float y)
{
	// EXP 바 렌더
	HDC memDC = CBitmapManager::GetInstance()->GetDC(__T("Exp"));

	CObj* player = CObjManager::GetInstance()->GetPlayer();
	if (nullptr == player) return;

	int lv = player->GetLV();
	float exp = player->GetEXP();
	float totalExp = lv * 100;

	float val = 300 * exp / totalExp;

	float revVal = 256 * val / 300.f;

	StretchBlt(hDC, x + 34, y + 146, val, 2, memDC, 0, 0, revVal, 2, SRCCOPY);

	// 글자 렌더
	StatTextRender(hDC, x, y);
}

void CInfoButton::EquipRender(HDC hDC, float x, float y)
{
	HDC memDC = CBitmapManager::GetInstance()->GetDC(__T("Items"));

	const CItem* item = CEquipManager::GetInstance()->GetHead();
	if (nullptr != item)
	{
		int ID = item->GetID();
		float xPos = x + 6;
		float yPos = y + 26;

		TransparentBlt(hDC, xPos, yPos, 24, 24, memDC, ID * 24, 0, 24, 24, RGB(255, 0, 255));

		EquipTextRender(hDC, xPos + 36, yPos + 2, item->GetKey());
	}

	item = CEquipManager::GetInstance()->GetArmor();
	if (nullptr != item)
	{
		int ID = item->GetID();
		float xPos = x + 6;
		float yPos = y + 26 + 31;

		TransparentBlt(hDC, xPos, yPos, 24, 24, memDC, ID * 24, 0, 24, 24, RGB(255, 0, 255));

		EquipTextRender(hDC, xPos + 36, yPos + 2, item->GetKey());
	}

	item = CEquipManager::GetInstance()->GetArms();
	if (nullptr != item)
	{
		int ID = item->GetID();
		float xPos = x + 6;
		float yPos = y + 26 + 62;

		TransparentBlt(hDC, xPos, yPos, 24, 24, memDC, ID * 24, 0, 24, 24, RGB(255, 0, 255));

		EquipTextRender(hDC, xPos + 36, yPos + 2, item->GetKey());
	}

	item = CEquipManager::GetInstance()->GetHand();
	if (nullptr != item)
	{
		int ID = item->GetID();
		float xPos = x + 6;
		float yPos = y + 26 + 93;

		TransparentBlt(hDC, xPos, yPos, 24, 24, memDC, ID * 24, 0, 24, 24, RGB(255, 0, 255));

		EquipTextRender(hDC, xPos + 36, yPos + 2, item->GetKey());
	}

	item = CEquipManager::GetInstance()->GetFoot();
	if (nullptr != item)
	{
		int ID = item->GetID();
		float xPos = x + 6;
		float yPos = y + 26 + 124;

		TransparentBlt(hDC, xPos, yPos, 24, 24, memDC, ID * 24, 0, 24, 24, RGB(255, 0, 255));

		EquipTextRender(hDC, xPos + 36, yPos + 2, item->GetKey());
	}

}

void CInfoButton::InvenRender(HDC hDC, float x, float y)
{
	HDC memDC = CBitmapManager::GetInstance()->GetDC(__T("Items"));

	int invenSize = CInvenManager::GetInstance()->GetSize();

	int max = (invenSize < 12) ? invenSize : 12;

	for (int i = 0; i < max; ++i)
	{
		float xPos = x + 11;
		float yPos = y + 42 + (25 * i);

		int id = CInvenManager::GetInstance()->GetInventory()->at(i)->GetID();
		int count = CInvenManager::GetInstance()->GetInventory()->at(i)->GetCount();

		TransparentBlt(hDC, xPos, yPos, 24, 24, memDC, id * 24, 0, 24, 24, RGB(255, 0, 255));
		InvenTextRender(hDC, xPos, yPos, i, count);
	}

	TCHAR buffer[30];

	float xPos = x + 56;
	float yPos = y + 381;

	wsprintf(buffer, __T("%d"), CInvenManager::GetInstance()->GetSize());
	TextOut(hDC, xPos, yPos, buffer, lstrlen(buffer));

	xPos += 210;

	CObj* player = CObjManager::GetInstance()->GetPlayer();
	if (nullptr == player) return;

	int seed = player->GetMoney();

	wsprintf(buffer, __T("%d"), seed);
	TextOut(hDC, xPos, yPos, buffer, lstrlen(buffer));
}

void CInfoButton::StatTextRender(HDC hDC, float x, float y)
{
	TCHAR buffer[30];

	CPlayer* player = dynamic_cast<CPlayer*>(CObjManager::GetInstance()->GetPlayer());
	if (nullptr == player) return;

	int level = player->GetLV();

	int offsetX = 216;
	int offsetY = 37;

	if (level < 10) offsetX += 3;

	// 레벨
	wsprintf(buffer, __T("%d"), level);
	TextOut(hDC, x + offsetX, y + offsetY, buffer, lstrlen(buffer));

	offsetX = 225;
	offsetY += 20;

	// 이름
	wsprintf(buffer, __T("막시민 리프크네"));
	TextOut(hDC, x + offsetX, y + offsetY, buffer, lstrlen(buffer));

	int hp = player->GetHP();
	int maxHP = MAXHP;

	offsetX = 240;
	offsetY = 182;

	// HP
	wsprintf(buffer, __T("%d / %d"), hp, maxHP);
	TextOut(hDC, x + offsetX, y + offsetY, buffer, lstrlen(buffer));

	int mp = player->GetMP();
	int maxMP = MAXMP;

	offsetY += 15;

	// MP
	wsprintf(buffer, __T("%d / %d"), mp, maxMP);
	TextOut(hDC, x + offsetX, y + offsetY, buffer, lstrlen(buffer));

	int sp = player->GetSP();
	int maxSP = MAXSP;

	offsetY += 15;

	// SP
	wsprintf(buffer, __T("%d / %d"), sp, maxSP);
	TextOut(hDC, x + offsetX, y + offsetY, buffer, lstrlen(buffer));

	int money = player->GetMoney();

	offsetY += 15;

	// Money
	wsprintf(buffer, __T("%d"), money);
	TextOut(hDC, x + offsetX, y + offsetY, buffer, lstrlen(buffer));

	// STAB
	offsetX = 64;
	offsetY = 182;

	int stab = player->GetLV() * 10;
	wsprintf(buffer, __T("%d"), stab);
	TextOut(hDC, x + offsetX, y + offsetY, buffer, lstrlen(buffer));

	// HACK
	offsetY += 15;

	int hack = player->GetLV() * 10;
	wsprintf(buffer, __T("%d"), hack);
	TextOut(hDC, x + offsetX, y + offsetY, buffer, lstrlen(buffer));

	// INT
	offsetY += 15;

	int INT = player->GetLV() * 5;
	wsprintf(buffer, __T("%d"), INT);
	TextOut(hDC, x + offsetX, y + offsetY, buffer, lstrlen(buffer));

	// DEF
	offsetY += 15;

	int DEF = player->GetLV() * 20;
	wsprintf(buffer, __T("%d"), DEF);
	TextOut(hDC, x + offsetX, y + offsetY, buffer, lstrlen(buffer));

	// MR
	offsetY += 15;

	int MR = player->GetLV() * 1;
	wsprintf(buffer, __T("%d"), MR);
	TextOut(hDC, x + offsetX, y + offsetY, buffer, lstrlen(buffer));

	// DEX
	offsetY += 15;

	int DEX = player->GetLV() * 5;
	wsprintf(buffer, __T("%d"), DEX);
	TextOut(hDC, x + offsetX, y + offsetY, buffer, lstrlen(buffer));

	// AGI
	offsetY += 15;

	int AGI = player->GetLV() * 5;
	wsprintf(buffer, __T("%d"), AGI);
	TextOut(hDC, x + offsetX, y + offsetY, buffer, lstrlen(buffer));
}

void CInfoButton::EquipTextRender(HDC hDC, float x, float y, const TCHAR* key)
{
	TCHAR buffer[30];

	wsprintf(buffer, key);
	TextOut(hDC, x, y, buffer, lstrlen(buffer));
}

void CInfoButton::InvenTextRender(HDC hDC, float x, float y, int i, int count)
{
	TCHAR buffer[30];

	float xPos = x + 34;
	float yPos = y + 8;

	wsprintf(buffer, CInvenManager::GetInstance()->GetInventory()->at(i)->GetKey());
	TextOut(hDC, xPos, yPos, buffer, lstrlen(buffer));

	xPos += 241;

	wsprintf(buffer, __T("%d"), count);
	TextOut(hDC, xPos, yPos, buffer, lstrlen(buffer));
}