#include "stdafx.h"
#include "Tile.h"
#include "MyBitmap.h"

CTile::CTile(float x, float y, int state) : m_state(state)
{
	m_info.xPos = x;
	m_info.yPos = y;
	m_info.xSize = TILECX;
	m_info.ySize = TILECY;

	UpdateRect();
}

CTile::~CTile()
{
	Release();
}

void CTile::Init()
{
}

int CTile::Update()
{
	return NO_EVENT;
}

void CTile::LateUpdate()
{
}

void CTile::Render(HDC hDC)
{
	HDC memDC = nullptr;

	auto transColor = RGB(255, 0, 255);
	int xPos = m_rect.left - CCamera::GetX();
	int yPos = m_rect.top - CCamera::GetY();
	int width = TILECX;
	int height = TILECY;

	switch (m_state)
	{
	case TILE_NORMAL:
		memDC = CBitmapManager::GetInstance()->GetDC(__T("Tile_Normal"));
		break;
	case TILE_FAIL:
		memDC = CBitmapManager::GetInstance()->GetDC(__T("Tile_Fail"));
		break;
	case TILE_TOWN:
		memDC = CBitmapManager::GetInstance()->GetDC(__T("Tile_Town"));
		break;
	case TILE_SHOP:
		memDC = CBitmapManager::GetInstance()->GetDC(__T("Tile_Shop"));
		break;
	case TILE_FIELD:
		memDC = CBitmapManager::GetInstance()->GetDC(__T("Tile_Field"));
		break;
	case TILE_DUN1:
		//memDC = CBitmapManager::GetInstance()->GetDC(__T("Tile_Dun1"));
		break;
	case TILE_DUN2:
		//memDC = CBitmapManager::GetInstance()->GetDC(__T("Tile_Dun2"));
		break;
	case TILE_BOSS:
		memDC = CBitmapManager::GetInstance()->GetDC(__T("Tile_Boss"));
		break;
	}

	if (nullptr == memDC) cout << "Error in Tile" << endl;

	GdiTransparentBlt(hDC, xPos, yPos, width, height, memDC, 0, 0, 30, 30, RGB(255, 0, 255));
}

void CTile::Release()
{
}
