#include "stdafx.h"
#include "Effect.h"

CEffect::CEffect(float x, float y, int cx, int cy, const TCHAR * key, int sceneFrame, int endFrame, DWORD frameSpeed, DWORD transColor, bool isOnce)
	: m_key(key), m_transColor(transColor), m_isOnce(isOnce)
{
	m_info.xPos = x;
	m_info.yPos = y;
	m_info.xSize = cx;
	m_info.ySize = cy;

	m_frame.startFrame = 0;
	m_frame.endFrame = endFrame;
	m_frame.frameTime = GetTickCount();
	m_frame.frameSpeed = frameSpeed;
	m_frame.sceneFrame = sceneFrame;

	Init();
}

CEffect::~CEffect()
{
	Release();
}

void CEffect::Init()
{
	UpdateRect();
}

int CEffect::Update()
{
	bool isEnd = UpdateFrame();

	if (m_isOnce && isEnd) return DEAD_EVENT;

	return NO_EVENT;
}

void CEffect::LateUpdate()
{
	UpdateRect();
}

void CEffect::Render(HDC hDC)
{
	HDC memDC = CBitmapManager::GetInstance()->GetDC(m_key);

	float xPos = m_info.xPos;
	float yPos = m_info.yPos;

	TransparentBlt(hDC, m_rect.left - CCamera::GetX(), m_rect.top - CCamera::GetY(), m_info.xSize, m_info.ySize, memDC, m_frame.startFrame * m_info.xSize, m_frame.sceneFrame * m_info.ySize, m_info.xSize, m_info.ySize, m_transColor);
}

void CEffect::Release()
{
}
