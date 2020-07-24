#include "stdafx.h"
#include "ShopUI.h"

CShopUI::CShopUI(int ID) : m_ID(ID)
{
	m_buyID = 1;
	m_sellID = 0;
	m_yesID = 0;
	m_cancleID = 0;

	m_shopListVal = 0;
	m_buyerListVal = 0;

	Init();
}

CShopUI::~CShopUI()
{
}

void CShopUI::Init()
{
	m_info.xSize = 384;
	m_info.ySize = 390;
}

int CShopUI::Update()
{
	ClickCheck();

	return NO_EVENT;
}

void CShopUI::LateUpdate()
{
}

void CShopUI::Render(HDC hDC)
{
	UIRender(hDC);
	ItemRender(hDC);
	TextRender(hDC);
}

void CShopUI::Release()
{
}

void CShopUI::ClickCheck()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_buyRect, pt)) m_buyID ^= 1;
	if (PtInRect(&m_sellRect, pt)) m_sellID ^= 1;
	if (PtInRect(&m_yesRect, pt)) m_yesID ^= 1;
	if (PtInRect(&m_cancleRect, pt)) m_cancleID ^= 1;
}

void CShopUI::TextRender(HDC hDC)
{
}

void CShopUI::ItemRender(HDC hDC)
{
}

void CShopUI::UIRender(HDC hDC)
{
	HDC memDC = CBitmapManager::GetInstance()->GetDC(__T("ShopWnd"));

	float startX = m_rect.left;
	float startY = m_rect.top;
	int width = m_rect.right - m_rect.left;
	int height = m_rect.bottom - m_rect.top;

	TransparentBlt(hDC, startX, startY, width, height, memDC, 0, 0, width, height, RGB(255, 0, 255));

	memDC = CBitmapManager::GetInstance()->GetDC(__T("Buy"));
	startX = m_buyRect.left;
	startY = m_buyRect.top;
	width = m_buyRect.right - m_buyRect.left;
	height = m_buyRect.bottom - m_buyRect.top;

	TransparentBlt(hDC, startX, startY, width, height, memDC, m_buyID * width, 0, width, height, RGB(255, 0, 255));

	memDC = CBitmapManager::GetInstance()->GetDC(__T("Sell"));
	startX = m_sellRect.left;
	startY = m_sellRect.top;
	width = m_sellRect.right - m_sellRect.left;
	height = m_sellRect.bottom - m_sellRect.top;

	TransparentBlt(hDC, startX, startY, width, height, memDC, m_sellID * width, 0, width, height, RGB(255, 0, 255));

	memDC = CBitmapManager::GetInstance()->GetDC(__T("OK"));
	startX = m_yesRect.left;
	startY = m_yesRect.top;
	width = m_yesRect.right - m_yesRect.left;
	height = m_yesRect.bottom - m_yesRect.top;

	TransparentBlt(hDC, startX, startY, width, height, memDC, m_yesID * width, 0, width, height, RGB(255, 0, 255));

	memDC = CBitmapManager::GetInstance()->GetDC(__T("Cancle"));
	startX = m_cancleRect.left;
	startY = m_cancleRect.top;
	width = m_cancleRect.right - m_cancleRect.left;
	height = m_cancleRect.bottom - m_cancleRect.top;

	TransparentBlt(hDC, startX, startY, width, height, memDC, m_cancleID * width, 0, width, height, RGB(255, 0, 255));
}

void CShopUI::RectInit()
{
	// ShopWnd
	m_rect.left = 200;
	m_rect.top = 100;
	m_rect.right = m_rect.left + m_info.xSize;
	m_rect.bottom = m_rect.top + m_info.ySize;

	// close
	m_closeRect.left = m_rect.left + 367;
	m_closeRect.top = m_rect.top + 4;
	m_closeRect.right = m_closeRect.left + 9;
	m_closeRect.bottom = m_closeRect.top + 9;

	// 구매 버튼
	m_buyRect.left = m_rect.left + 5 - 21;
	m_buyRect.top = m_rect.top + 6;
	m_buyRect.right = m_buyRect.left + 21;
	m_buyRect.bottom = m_buyRect.top + 70;

	// 판매 버튼
	m_sellRect.left = m_buyRect.left;
	m_sellRect.top = m_buyRect.bottom + 1;
	m_sellRect.right = m_sellRect.left + 21;
	m_sellRect.bottom = m_sellRect.top + 70;

	// 예 버튼
	m_yesRect.left = m_rect.left + 236;
	m_yesRect.top = m_rect.top + 347;
	m_yesRect.right = m_yesRect.left + 32;
	m_yesRect.bottom = m_yesRect.top + 14;

	// 취소 버튼
	m_cancleRect.left = m_yesRect.right + 42;
	m_cancleRect.top = m_yesRect.top;
	m_cancleRect.right = m_cancleRect.left + 32;
	m_cancleRect.bottom = m_cancleRect.top + 14;
}
