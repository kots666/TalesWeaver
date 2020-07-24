#include "stdafx.h"
#include "WidthButton.h"

CWidthButton::CWidthButton(float x, float y, int cx, int cy, const TCHAR * key)
	: m_key(key)
{
	m_info.xPos = x;
	m_info.yPos = y;
	m_info.xSize = cx;
	m_info.ySize = cy;

	m_ID = 1;

	Init();
}

CWidthButton::~CWidthButton()
{
	Release();
}

void CWidthButton::Init()
{
	UpdateRect();
}

int CWidthButton::Update()
{
	return NO_EVENT;
}

void CWidthButton::LateUpdate()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_rect, pt))
	{
		m_ID = 0;

		if (CKeyManager::GetInstance()->isKeyDown(KEY_LBUTTON))
		{
			if (!lstrcmp(m_key, __T("Start")))
			{
				CSceneManager::GetInstance()->ChangeScene(CSceneManager::Scene_Town, TITLE_TO_TOWN);
				CObjManager::GetInstance()->RemoveObject(OBJ::BUTTON);
				return;
			}
			else if (!lstrcmp(m_key, __T("Credit")))
			{
				// Tile
				CBitmapManager::GetInstance()->InsertBitmap(__T("TILE_NORMAL"), __T("./Image/Tile/tile_0.bmp"));
				CBitmapManager::GetInstance()->InsertBitmap(__T("TILE_FAIL"), __T("./Image/Tile/tile_1.bmp"));
				CBitmapManager::GetInstance()->InsertBitmap(__T("TILE_TOWN"), __T("./Image/Tile/tile_2.bmp"));
				CBitmapManager::GetInstance()->InsertBitmap(__T("TILE_SHOP"), __T("./Image/Tile/tile_3.bmp"));
				CBitmapManager::GetInstance()->InsertBitmap(__T("TILE_FIELD"), __T("./Image/Tile/tile_4.bmp"));
				//CBitmapManager::GetInstance()->InsertBitmap(__T("TILE_DUNGEON1"), __T("./Image/Tile/tile_5.bmp"));
				//CBitmapManager::GetInstance()->InsertBitmap(__T("TILE_DUNGEON2"), __T("./Image/Tile/tile_6.bmp"));
				CBitmapManager::GetInstance()->InsertBitmap(__T("TILE_BOSS"), __T("./Image/Tile/tile_7.bmp"));

				CSceneManager::GetInstance()->ChangeScene(CSceneManager::Scene_Edit, NULL);
				CObjManager::GetInstance()->RemoveObject(OBJ::BUTTON);
				return;
			}
			else if (!lstrcmp(m_key, __T("Exit")))
			{
				CSceneManager::GetInstance()->ChangeScene(CSceneManager::Scene_End, NULL);
				CObjManager::GetInstance()->RemoveObject(OBJ::BUTTON);
				return;
			}
		}
	}
	else m_ID = 1;
	UpdateRect();
}

void CWidthButton::Render(HDC hDC)
{
	HDC memDC = CBitmapManager::GetInstance()->GetDC(m_key);

	int originX = m_rect.left;
	int originY = m_rect.top;
	int width = m_info.xSize;
	int height = m_info.ySize;

	BitBlt(hDC, originX, originY, width, height, memDC, m_ID * 115, 0, SRCCOPY);
}

void CWidthButton::Release()
{
}
