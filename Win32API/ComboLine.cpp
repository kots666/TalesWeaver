#include "stdafx.h"
#include "ComboLine.h"

CComboLine::CComboLine() : m_spawnTime(GetTickCount())
{
}

CComboLine::~CComboLine()
{
}

void CComboLine::Init()
{
}

int CComboLine::Update()
{
	if (m_isDead) return DEAD_EVENT;

	return NO_EVENT;
}

void CComboLine::LateUpdate()
{
	if (m_spawnTime + 50 < GetTickCount()) m_isDead = true;
}

void CComboLine::Render(HDC hDC)
{
	HDC memDC = CBitmapManager::GetInstance()->GetDC(__T("Comline"));

	TransparentBlt(hDC, 0, 0, WINCX, WINCY, memDC, 0, 0, WINCX, WINCY, RGB(0, 0, 0));
}

void CComboLine::Release()
{
}
