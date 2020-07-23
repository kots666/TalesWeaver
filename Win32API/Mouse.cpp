#include "stdafx.h"
#include "Mouse.h"
#include "MyBitmap.h"

CMouse::CMouse()
{
	Init();
}

CMouse::~CMouse()
{
}

void CMouse::Init()
{
	ShowCursor(false);
	m_info.xSize = 32;
	m_info.ySize = 32;

	m_state = MOUSE::IDLE;
	m_frame.startFrame = 0;
	m_frame.endFrame = 13;
	m_frame.sceneFrame = 0;
	m_frame.frameTime = GetTickCount();
	m_frame.frameSpeed = 50;

	m_isClicked = false;
}

int CMouse::Update()
{
	// 마우스 기본적인 움직임
	POINT pos = {};
	GetCursorPos(&pos);
	ScreenToClient(g_hWnd, &pos);

	m_info.xPos = float(pos.x);
	m_info.yPos = float(pos.y);

	int xTile = (m_info.xPos + CCamera::GetX()) / TILECX;
	int yTile = (m_info.yPos + CCamera::GetY()) / TILECY;

	int index = xTile + (TILEX * yTile);

	/*
	if (TILE_FAIL < CCollisionManager::GetCollideTileState(index)) ChangeAction(MOUSE::PORTAL);
	else if (TILE_FAIL == CCollisionManager::GetCollideTileState(index)) ChangeAction(MOUSE::FAIL);
	else ChangeAction(MOUSE::IDLE);
	*/

	return NO_EVENT;
}

void CMouse::LateUpdate()
{
	// 클릭했는지 체크
	CheckIdleClick();

	// 클릭했을 때의 마우스
	if (m_isClicked)
	{
		if (FRAME_END == UpdateFrame())
		{
			m_isClicked = false;
			m_frame.startFrame = 0;
			m_frame.endFrame = 13;
			m_frame.sceneFrame = 0;
			m_frame.frameTime = GetTickCount();
		}
	}
	else
	{
		UpdateFrame();
	}

	UpdateRect();
}

void CMouse::Render(HDC hDC)
{
	HDC memDC = nullptr;

	int xPos = m_info.xPos;
	int yPos = m_info.yPos;
	int width = m_info.xSize;
	int height = m_info.ySize;

	switch (m_state)
	{
	case MOUSE::IDLE:
		memDC = CBitmapManager::GetInstance()->GetDC(__T("Mouse_Idle"));
		break;
	case MOUSE::ATTACK:
		memDC = CBitmapManager::GetInstance()->GetDC(__T("Mouse_Attack"));
		break;
	case MOUSE::PORTAL:
		memDC = CBitmapManager::GetInstance()->GetDC(__T("Mouse_Portal"));
		break;
	case MOUSE::FAIL:
		memDC = CBitmapManager::GetInstance()->GetDC(__T("Mouse_Fail"));
		break;
	}

	GdiTransparentBlt(hDC, xPos, yPos, width, height, memDC, width * m_frame.startFrame, height * m_frame.sceneFrame, width, height, RGB(0,0,0));
}

void CMouse::Release()
{
}

void CMouse::CheckIdleClick()
{
	if (m_state == MOUSE::IDLE && !m_isClicked) {
		if (CKeyManager::GetInstance()->isKeyDown(KEY_LBUTTON))
		{
			m_isClicked = true;
			m_frame.startFrame = 0;
			m_frame.endFrame = 2;
			m_frame.sceneFrame = 1;
			m_frame.frameTime = GetTickCount();
		}

	}
}

void CMouse::ChangeAction(MOUSE::STATE state)
{
	if (m_state != state)
	{
		m_state = state;
		switch (state)
		{
		case MOUSE::IDLE :
			m_info.xSize = 32;
			m_info.ySize = 32;
			m_frame.startFrame = 0;
			m_frame.endFrame = 13;
			m_frame.sceneFrame = 0;
			m_frame.frameTime = GetTickCount();
			m_frame.frameSpeed = 50;
			break;
		case MOUSE::ATTACK:
			break;
		case MOUSE::PORTAL :
			m_info.xSize = 27;
			m_info.ySize = 22;
			m_frame.startFrame = 0;
			m_frame.endFrame = 4;
			m_frame.sceneFrame = 0;
			m_frame.frameTime = GetTickCount();
			m_frame.frameSpeed = 100;
			break;
		case MOUSE::FAIL:
			m_info.xSize = 32;
			m_info.ySize = 32;
			m_frame.startFrame = 0;
			m_frame.endFrame = 12;
			m_frame.sceneFrame = 0;
			m_frame.frameTime = GetTickCount();
			m_frame.frameSpeed = 50;
			break;
		}
	}
}
