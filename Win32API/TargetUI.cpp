#include "stdafx.h"
#include "TargetUI.h"


CTargetUI::CTargetUI(CObj* target, int cx, int cy, const TCHAR * key, int width, int height, int scene, int end, DWORD frameSpeed, DWORD transColor, bool isOnce, int offsetX, int offsetY)
	: m_target(target), m_key(key), m_color(transColor), m_width(width), m_height(height), m_isOnce(isOnce),
	m_offsetX(offsetX), m_offsetY(offsetY)
{
	m_info.xSize = cx;
	m_info.ySize = cy;

	m_frame.startFrame = 0;
	m_frame.endFrame = end;
	m_frame.sceneFrame = scene;
	m_frame.frameTime = GetTickCount();
	m_frame.frameSpeed = frameSpeed;


	Init();
}

CTargetUI::~CTargetUI()
{
}

void CTargetUI::Init()
{
	if (nullptr == m_target) return;
	INFO playerInfo = m_target->GetInfo();

	UpdateRect();
}

int CTargetUI::Update()
{
	bool isEnd = UpdateFrame();

	if (isEnd && m_isOnce) m_isDead = true;

	if (m_isDead) return DEAD_EVENT;

	if (nullptr == m_target) return DEAD_EVENT;
	INFO playerInfo = m_target->GetInfo();

	m_info.xPos = playerInfo.xPos + m_offsetX;
	m_info.yPos = playerInfo.yPos - m_offsetY;

	return NO_EVENT;
}

void CTargetUI::LateUpdate()
{
	UpdateRect();
}

void CTargetUI::Render(HDC hDC)
{
	HDC memDC = CBitmapManager::GetInstance()->GetDC(m_key);

	TransparentBlt(hDC, m_rect.left - CCamera::GetX(), m_rect.top - CCamera::GetY(), m_info.xSize, m_info.ySize, memDC, m_frame.startFrame * m_width, m_frame.sceneFrame * m_height, m_width, m_height, m_color);
}

void CTargetUI::Release()
{
}
