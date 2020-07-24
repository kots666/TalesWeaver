#include "stdafx.h"
#include "NPC.h"
#include "ShopUI.h"

CNPC::CNPC(float x, float y, int cx, int cy, int ID) : m_ID(ID)
{
	m_info.xPos = x;
	m_info.yPos = y;
	m_info.xSize = cx;
	m_info.ySize = cy;

	Init();
}

CNPC::~CNPC()
{
}

void CNPC::Init()
{
	if (m_ID)
	{
		m_frame.startFrame = 0;
		m_frame.endFrame = 11;
		m_frame.sceneFrame = 0;
		m_frame.frameTime = GetTickCount();
		m_frame.frameSpeed = 100;
	}

	UpdateRect();
}

int CNPC::Update()
{
	if (m_isDead) return DEAD_EVENT;

	if(m_ID) UpdateFrame();

	ClickCheck();

	return NO_EVENT;
}

void CNPC::LateUpdate()
{
}

void CNPC::Render(HDC hDC)
{
	if (m_ID)
	{
		HDC memDC = CBitmapManager::GetInstance()->GetDC(__T("NPC"));

		// 144 144 168

		float startX = m_info.xPos - (96 / 2);
		float startY = m_info.yPos - (91 / 2.f);

		TransparentBlt(hDC, startX - CCamera::GetX(), startY - CCamera::GetY(), 96, 91, memDC, m_frame.startFrame * 96, 0, 96, 91, RGB(144, 144, 168));
	}
}

void CNPC::Release()
{
}

void CNPC::ClickCheck()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	RECT rc = m_rect;
	pt.x += CCamera::GetX();
	pt.y += CCamera::GetY();

	if (PtInRect(&rc, pt))
	{
		if (CKeyManager::GetInstance()->isKeyDown(KEY_LBUTTON))
		{
			OpenShopUI();
		}
	}
}

void CNPC::OpenShopUI()
{
	CObjManager::GetInstance()->RemoveObject(OBJ::SHOP);
	CObj* newShop = new CShopUI(m_ID);
	CObjManager::GetInstance()->AddObject(newShop, OBJ::SHOP);
}
