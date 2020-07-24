#include "stdafx.h"
#include "InfoButton.h"
#include "Player.h"

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
		if (CKeyManager::GetInstance()->isKeyDown(KEY_LBUTTON))
		{
			m_isChildOpen ^= true;
			m_ID ^= 1;
		}
	}

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
		if (!(lstrcmp(m_childKey, __T("Equip_Info")))) EquipRender(hDC);
		if (!(lstrcmp(m_childKey, __T("Inven_Info")))) InvenRender(hDC);
	}
}

void CInfoButton::Release()
{
}

void CInfoButton::StatRender(HDC hDC, float x, float y)
{
	// EXP ¹Ù ·»´õ
	HDC memDC = CBitmapManager::GetInstance()->GetDC(__T("Exp"));

	CObj* player = CObjManager::GetInstance()->GetPlayer();
	if (nullptr == player) return;

	int lv = player->GetLV();
	float exp = player->GetEXP();
	float totalExp = lv * 100;

	float val = 300 * exp / totalExp;

	float revVal = 256 * val / 300.f;

	StretchBlt(hDC, x + 34, y + 146, val, 2, memDC, 0, 0, revVal, 2, SRCCOPY);

	// ±ÛÀÚ ·»´õ
	StatTextRender(hDC, x, y);
}

void CInfoButton::EquipRender(HDC hDC)
{
}

void CInfoButton::InvenRender(HDC hDC)
{
}

void CInfoButton::StatTextRender(HDC hDC, float x, float y)
{
	HDC memDC = CreateCompatibleDC(hDC);
	HBITMAP bmp = CreateCompatibleBitmap(hDC, WINCX, WINCY);
	HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, bmp);

	HFONT myFont = CreateFont(11, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, __T("±¼¸²"));
	HFONT oldFont = (HFONT)SelectObject(memDC, myFont);

	SetBkColor(memDC, RGB(0, 0, 0));
	SetTextColor(memDC, RGB(255, 255, 255));

	BitBlt(memDC, 0, 0, WINCX, WINCY, hDC, 0, 0, SRCCOPY);

	TCHAR buffer[30];

	CPlayer* player = dynamic_cast<CPlayer*>(CObjManager::GetInstance()->GetPlayer());
	if (nullptr == player) return;

	int level = player->GetLV();

	int offsetX = 216;
	int offsetY = 37;

	if (level < 10) offsetX += 3;

	// ·¹º§
	wsprintf(buffer, __T("%d"), level);
	TextOut(memDC, x + offsetX, y + offsetY, buffer, lstrlen(buffer));

	offsetX = 225;
	offsetY += 20;

	// ÀÌ¸§
	wsprintf(buffer, __T("¸·½Ã¹Î ¸®ÇÁÅ©³×"));
	TextOut(memDC, x + offsetX, y + offsetY, buffer, lstrlen(buffer));

	int hp = player->GetHP();
	int maxHP = 100;

	offsetX = 240;
	offsetY = 182;

	// HP
	wsprintf(buffer, __T("%d / %d"), hp, maxHP);
	TextOut(memDC, x + offsetX, y + offsetY, buffer, lstrlen(buffer));

	int mp = player->GetMP();
	int maxMP = 100;

	offsetY += 15;

	// MP
	wsprintf(buffer, __T("%d / %d"), mp, maxMP);
	TextOut(memDC, x + offsetX, y + offsetY, buffer, lstrlen(buffer));

	int sp = player->GetSP();
	int maxSP = 100;

	offsetY += 15;

	// SP
	wsprintf(buffer, __T("%d / %d"), sp, maxSP);
	TextOut(memDC, x + offsetX, y + offsetY, buffer, lstrlen(buffer));

	int money = player->GetMoney();

	offsetY += 15;

	// Money
	wsprintf(buffer, __T("%d"), money);
	TextOut(memDC, x + offsetX, y + offsetY, buffer, lstrlen(buffer));

	TransparentBlt(hDC, 0, 0, WINCX, WINCY, memDC, 0, 0, WINCX, WINCY, RGB(0, 0, 0));

	SelectObject(memDC, oldFont);
	DeleteObject(myFont);
	SelectObject(memDC, oldBmp);
	DeleteObject(bmp);
	DeleteDC(memDC);
}
