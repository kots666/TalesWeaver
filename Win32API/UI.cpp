#include "stdafx.h"
#include "UI.h"
#include "MyBitmap.h"

CUI::CUI(float x, float y, int cx, int cy, const TCHAR * key, int * sx, bool isTrans, int * state, int maxState, int offset, DWORD color)
	: m_startX(sx), m_key(key), m_isTrans(isTrans),
	m_state(state), m_maxState(maxState), m_offset(offset), m_color(color)
{
	m_info.xPos = x;
	m_info.yPos = y;
	m_info.xSize = cx;
	m_info.ySize = cy;

	Init();
}

CUI::~CUI()
{
	Release();
}

void CUI::Init()
{
	UpdateRect();
}

int CUI::Update()
{
	return NO_EVENT;
}

void CUI::LateUpdate()
{
	UpdateRect();
}

void CUI::Render(HDC hDC)
{
	HDC memDC = CBitmapManager::GetInstance()->GetDC(m_key);

	float x = m_info.xPos;
	float y = m_info.yPos;
	int width = m_info.xSize;
	int height = m_info.ySize;

	int start = 0;

	if (nullptr != m_startX)
		start = *m_startX;

	if (!lstrcmp(m_key, __T("Player_Hp")))
	{
		int value = ((float)*m_state / MAXHP) * 84.f;

		TransparentBlt(hDC, x, y, value, height, memDC, start, 0, value, height, m_color);
	}
	else if (!lstrcmp(m_key, __T("Player_Mp")))
	{
		int value = ((float)*m_state / MAXMP) * 84.f;

		TransparentBlt(hDC, x, y, value, height, memDC, start, 0, value, height, m_color);
	}
	else if (!lstrcmp(m_key, __T("Player_Sp")))
	{
		int value = ((float)*m_state / MAXSP) * 84.f;

		TransparentBlt(hDC, x, y, value, height, memDC, start, 0, value, height, m_color);
	}

	if (nullptr != m_state)
	{
		int state = *m_state;
		int value = m_offset * (state / (float)m_maxState);

		if (m_isTrans)
			TransparentBlt(hDC, x, y, value, height, memDC, start, 0, value, height, m_color);
		else
			BitBlt(hDC, x, y, value, height, memDC, start, 0, SRCCOPY);
	}
	else
	{
		if (m_isTrans)
			TransparentBlt(hDC, x, y, width, height, memDC, start * width, 0, width, height, m_color);
		else
			BitBlt(hDC, x, y, width, height, memDC, start * width, 0, SRCCOPY);
	}
}

void CUI::Release()
{
}
