#include "stdafx.h"
#include "MyBitmap.h"


CMyBitmap::CMyBitmap()
{
}


CMyBitmap::~CMyBitmap()
{
	Release();
}

void CMyBitmap::Load(const TCHAR * path)
{
	HDC dc = GetDC(g_hWnd);
	m_memDC = CreateCompatibleDC(dc);

	ReleaseDC(g_hWnd, dc);

	m_bitmap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	BITMAP bm;
	GetObject(m_bitmap, sizeof(BITMAP), (LPVOID)&bm);

	m_width = (int)bm.bmWidth;
	m_height = (int)bm.bmHeight;

	m_oldBitmap = (HBITMAP)SelectObject(m_memDC, m_bitmap);
}

void CMyBitmap::Release()
{
	SelectObject(m_memDC, m_oldBitmap);
	DeleteObject(m_bitmap);
	DeleteDC(m_memDC);
}
