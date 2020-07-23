#include "stdafx.h"
#include "SceneEdit.h"
#include "MyBitmap.h"

CSceneEdit::CSceneEdit()
	: m_editState(EDIT::READY), m_selectTile(TILE_NORMAL),
	m_scrollX(0.f), m_scrollY(0.f),
	m_maxX(0.f), m_maxY(0.f), m_speed(10.f)
{
	Init();
}

CSceneEdit::~CSceneEdit()
{
	Release();
}

void CSceneEdit::Init()
{
	CCamera::Init();
}

void CSceneEdit::Update()
{
	ScrollTile();
}

void CSceneEdit::LateUpdate()
{
	if (EDIT::READY == m_editState) ReadyPick();
	else PickTile();
}

void CSceneEdit::Render(HDC hDC)
{	
	if (EDIT::READY == m_editState)
	{
		ReadyRender(hDC);
	}
	else
	{
		OtherRender(hDC);

		CTileManager::GetInstance()->Render(hDC);

		LastRender(hDC);
	}
}

void CSceneEdit::Release()
{
}

void CSceneEdit::ReadyPick()
{
	if (CKeyManager::GetInstance()->isKeyDown(KEY_BACK))
	{
		CSceneManager::GetInstance()->ChangeScene(CSceneManager::Scene_Title, NULL);
		return;
	}

	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	RECT townRect = { 0, 0, WINCX / 2, WINCY / 2 };
	RECT shopRect = { WINCX / 2, 0, WINCX, WINCY / 2 };
	RECT fieldRect = { 0, WINCY / 2, WINCX / 2, WINCY };
	RECT bossRect = { WINCX / 2, WINCY / 2, WINCX, WINCY };

	if (PtInRect(&townRect, pt))
	{
		if (CKeyManager::GetInstance()->isKeyDown(KEY_LBUTTON))
		{
			m_editState = EDIT::TOWN;
			m_maxX = CBitmapManager::GetInstance()->GetMyBitmap(__T("Town"))->GetWidth() - (WINCX - 150);
			m_maxY = CBitmapManager::GetInstance()->GetMyBitmap(__T("Town"))->GetHeight() - WINCY;
			CTileManager::GetInstance()->LoadData(m_editState);
		}
	}
	else if (PtInRect(&shopRect, pt))
	{
		if (CKeyManager::GetInstance()->isKeyDown(KEY_LBUTTON))
		{
			m_editState = EDIT::SHOP;
			m_maxX = CBitmapManager::GetInstance()->GetMyBitmap(__T("Shop"))->GetWidth() - (WINCX - 150);
			m_maxY = CBitmapManager::GetInstance()->GetMyBitmap(__T("Shop"))->GetHeight() - WINCY;
			CTileManager::GetInstance()->LoadData(m_editState);
		}
	}
	else if (PtInRect(&fieldRect, pt))
	{
		if (CKeyManager::GetInstance()->isKeyDown(KEY_LBUTTON))
		{
			m_editState = EDIT::FIELD;
			m_maxX = CBitmapManager::GetInstance()->GetMyBitmap(__T("Field"))->GetWidth() - (WINCX - 150);
			m_maxY = CBitmapManager::GetInstance()->GetMyBitmap(__T("Field"))->GetHeight() - WINCY;
			CTileManager::GetInstance()->LoadData(m_editState);
		}
	}
	else if (PtInRect(&bossRect, pt))
	{
		if (CKeyManager::GetInstance()->isKeyDown(KEY_LBUTTON))
		{
			m_editState = EDIT::BOSS;
			m_maxX = CBitmapManager::GetInstance()->GetMyBitmap(__T("Boss"))->GetWidth() - (WINCX - 150);
			m_maxY = CBitmapManager::GetInstance()->GetMyBitmap(__T("Boss"))->GetHeight() - WINCY;
			CTileManager::GetInstance()->LoadData(m_editState);
		}
	}
}

void CSceneEdit::PickTile()
{
	if (CKeyManager::GetInstance()->isKeyDown(KEY_1)) m_selectTile = TILE_NORMAL;
	if (CKeyManager::GetInstance()->isKeyDown(KEY_2)) m_selectTile = TILE_FAIL;
	if (CKeyManager::GetInstance()->isKeyDown(KEY_3)) m_selectTile = TILE_TOWN;
	if (CKeyManager::GetInstance()->isKeyDown(KEY_4)) m_selectTile = TILE_SHOP;
	if (CKeyManager::GetInstance()->isKeyDown(KEY_5)) m_selectTile = TILE_FIELD;
	if (CKeyManager::GetInstance()->isKeyDown(KEY_6)) m_selectTile = TILE_BOSS;

	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	RECT rc = { 0, 0, WINCX - 150, WINCY };
	RECT SaveRC = { 3 + WINCX - 150, 35, 49 + WINCX - 150, 59 };
	RECT LoadRC = { 52 + WINCX - 150, 35, 99 + WINCX - 150, 59 };
	RECT ExitRC = { 102 + WINCX - 150, 35, 147 + WINCX - 150, 59 };

	if (PtInRect(&rc, pt))
	{
		if (CKeyManager::GetInstance()->isKeyPressing(KEY_LBUTTON))
		{
			int xPos = pt.x + CCamera::GetX();
			int yPos = pt.y + CCamera::GetY();

			int tileX = xPos / TILECX;
			int tileY = yPos / TILECY;

			int index = tileX + (tileY * TILEX);

			CTileManager::GetInstance()->ChangeTile(index, m_selectTile);
		}
	}
	else if (PtInRect(&SaveRC, pt))
	{
		if (CKeyManager::GetInstance()->isKeyDown(KEY_LBUTTON))
		{
			CTileManager::GetInstance()->SaveData(m_editState);
		}
	}
	else if (PtInRect(&LoadRC, pt))
	{
		if (CKeyManager::GetInstance()->isKeyDown(KEY_LBUTTON))
		{
			CTileManager::GetInstance()->LoadData(m_editState);
		}
	}
	else if (PtInRect(&ExitRC, pt))
	{
		if (CKeyManager::GetInstance()->isKeyDown(KEY_LBUTTON))
		{
			m_editState = EDIT::READY;
		}
	}
}

void CSceneEdit::ScrollTile()
{
	if (EDIT::READY != m_editState)
	{
		if (CKeyManager::GetInstance()->isKeyPressing(KEY_UP))
		{
			if (0 <= CCamera::GetY() - m_speed) CCamera::AddY(-m_speed);
		}
		if (CKeyManager::GetInstance()->isKeyPressing(KEY_LEFT))
		{
			if (0 <= CCamera::GetX() - m_speed) CCamera::AddX(-m_speed);
		}
		if (CKeyManager::GetInstance()->isKeyPressing(KEY_DOWN))
		{
			if (m_maxY >= CCamera::GetY() + m_speed) CCamera::AddY(m_speed);
		}
		if (CKeyManager::GetInstance()->isKeyPressing(KEY_RIGHT))
		{
			if (m_maxX >= CCamera::GetX() + m_speed) CCamera::AddX(m_speed);
		}
	}
}

void CSceneEdit::ReadyRender(HDC hDC)
{
	TCHAR key[4][16] = { __T("Mini_Town"), __T("Mini_Shop"), __T("Mini_Field"), __T("Mini_Boss") };

	for (int i = 0; i < 4; ++i)
	{
		HDC memDC = CBitmapManager::GetInstance()->GetDC(key[i]);
		int width = CBitmapManager::GetInstance()->GetMyBitmap(key[i])->GetWidth();
		int height = CBitmapManager::GetInstance()->GetMyBitmap(key[i])->GetHeight();

		int x = (i & 1) * WINCX / 2;
		int y = (i / 2) * WINCY / 2;

		BitBlt(hDC, x, y, x + WINCX / 2, y + WINCY / 2, memDC, 0, 0, SRCCOPY);
	}
}

void CSceneEdit::OtherRender(HDC hDC)
{
	HDC memDC = nullptr;
	int width = 0;
	int height = 0;

	switch (m_editState)
	{
	case EDIT::TOWN:
		memDC = CBitmapManager::GetInstance()->GetDC(__T("Town"));
		width = CBitmapManager::GetInstance()->GetMyBitmap(__T("Town"))->GetWidth();
		height = CBitmapManager::GetInstance()->GetMyBitmap(__T("Town"))->GetHeight();
		break;
	case EDIT::SHOP:
		memDC = CBitmapManager::GetInstance()->GetDC(__T("Shop"));
		width = CBitmapManager::GetInstance()->GetMyBitmap(__T("Shop"))->GetWidth();
		height = CBitmapManager::GetInstance()->GetMyBitmap(__T("Shop"))->GetHeight();
		break;
	case EDIT::FIELD:
		memDC = CBitmapManager::GetInstance()->GetDC(__T("Field"));
		width = CBitmapManager::GetInstance()->GetMyBitmap(__T("Field"))->GetWidth();
		height = CBitmapManager::GetInstance()->GetMyBitmap(__T("Field"))->GetHeight();
		break;
	case EDIT::BOSS:
		memDC = CBitmapManager::GetInstance()->GetDC(__T("Boss"));
		width = CBitmapManager::GetInstance()->GetMyBitmap(__T("Boss"))->GetWidth();
		height = CBitmapManager::GetInstance()->GetMyBitmap(__T("Boss"))->GetHeight();
		break;
	}

	BitBlt(hDC, 0, 0, WINCX - 150, WINCY, memDC, CCamera::GetX(), CCamera::GetY(), SRCCOPY);
}

void CSceneEdit::LastRender(HDC hDC)
{
	HDC memDC = CBitmapManager::GetInstance()->GetDC(__T("Tile_Edit"));
	
	BitBlt(hDC, WINCX - 150, 0, 150, WINCY, memDC, 0, 0, SRCCOPY);

	switch (m_selectTile)
	{
	case TILE_NORMAL: memDC = CBitmapManager::GetInstance()->GetDC(__T("Tile_Normal")); break;
	case TILE_FAIL: memDC = CBitmapManager::GetInstance()->GetDC(__T("Tile_Fail")); break;
	case TILE_TOWN: memDC = CBitmapManager::GetInstance()->GetDC(__T("Tile_Town")); break;
	case TILE_SHOP: memDC = CBitmapManager::GetInstance()->GetDC(__T("Tile_Shop")); break;
	case TILE_FIELD: memDC = CBitmapManager::GetInstance()->GetDC(__T("Tile_Field")); break;
	//case TILE_DUN1: memDC = CBitmapManager::GetInstance()->GetDC(__T("Tile_Dun1")); break;
	//case TILE_DUN2: memDC = CBitmapManager::GetInstance()->GetDC(__T("Tile_Dun2")); break;
	case TILE_BOSS: memDC = CBitmapManager::GetInstance()->GetDC(__T("Tile_Boss")); break;
	}

	BitBlt(hDC, WINCX - 149, 1, 30, 30, memDC, 0, 0, SRCCOPY);
}
