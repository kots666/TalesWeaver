#include "stdafx.h"
#include "MotionTrail.h"

CMotionTrail::CMotionTrail(float x, float y, int width, int height, float gapX, float gapY, int frame, int scene, DWORD transColor, const TCHAR * key)
	: m_gapX(gapX), m_gapY(gapY), m_transColor(transColor), m_key(key), m_alpha(255), m_flyY(0.f)
{
	m_info.xPos = x;
	m_info.yPos = y;
	m_info.xSize = width;
	m_info.ySize = height;
	m_gapX = gapX;
	m_gapY = gapY;
	m_frame.startFrame = frame;
	m_frame.sceneFrame = scene;

	Init();
}

CMotionTrail::~CMotionTrail()
{
	Release();
}

void CMotionTrail::Init()
{
	m_bf.AlphaFormat = 0;
	m_bf.BlendFlags = 0;
	m_bf.BlendOp = AC_SRC_OVER;

	UpdateRect();
}

int CMotionTrail::Update()
{
	if (m_isDead) return DEAD_EVENT;

	m_flyY -= 1.5f;
	m_alpha -= 5;

	m_bf.SourceConstantAlpha = m_alpha;

	return NO_EVENT;
}

void CMotionTrail::LateUpdate()
{
	if (m_alpha <= 0) m_isDead = true;
	UpdateRect();
}

void CMotionTrail::Render(HDC hDC)
{
	float xPos = m_info.xPos - m_gapX;
	float yPos = m_info.yPos - m_gapY;
	int width = m_info.xSize;
	int height = m_info.ySize;

	HDC memDC = CreateCompatibleDC(hDC);
	HBITMAP memBmp = CreateCompatibleBitmap(hDC, WINCX, WINCY);
	HBITMAP oldMemBmp = (HBITMAP)SelectObject(memDC, memBmp);

	BitBlt(memDC, 0, 0, WINCX, WINCY, hDC, 0, 0, SRCCOPY);

	HDC alphaDC = CreateCompatibleDC(memDC);
	HBITMAP alphaBmp = CreateCompatibleBitmap(memDC, WINCX, WINCY);
	HBITMAP oldAlphaBmp = (HBITMAP)SelectObject(alphaDC, alphaBmp);

	BitBlt(alphaDC, 0, 0, WINCX, WINCY, memDC, 0, 0, SRCCOPY);

	HDC transDC = CBitmapManager::GetInstance()->GetDC(m_key);

	TransparentBlt(alphaDC, xPos - CCamera::GetX(), yPos + m_flyY - CCamera::GetY(), width, height, transDC, m_frame.startFrame * width, m_frame.sceneFrame * height, width, height, m_transColor);

	AlphaBlend(memDC, xPos - CCamera::GetX(), yPos + m_flyY - CCamera::GetY(), width, height, alphaDC, xPos - CCamera::GetX(), yPos + m_flyY - CCamera::GetY(), width, height, m_bf);

	SelectObject(alphaDC, oldAlphaBmp);
	DeleteObject(alphaBmp);
	DeleteDC(alphaDC);

	BitBlt(hDC, 0, 0, WINCX, WINCY, memDC, 0, 0, SRCCOPY);

	SelectObject(memDC, oldMemBmp);
	DeleteObject(memBmp);
	DeleteDC(memDC);

	TransparentBlt(hDC, xPos - CCamera::GetX(), yPos - CCamera::GetY(), width, height, transDC, m_frame.startFrame * width, m_frame.sceneFrame * height, width, height, m_transColor);
}

void CMotionTrail::Release()
{
}
