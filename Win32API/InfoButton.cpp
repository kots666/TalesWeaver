#include "stdafx.h"
#include "InfoButton.h"

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
	}
}

void CInfoButton::Release()
{
}
