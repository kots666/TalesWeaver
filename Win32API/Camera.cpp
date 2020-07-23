#include "stdafx.h"
#include "Camera.h"

int CCamera::m_x = 0;
int CCamera::m_y = 0;
int CCamera::m_maxX = 0;
int CCamera::m_maxY = 0;

void CCamera::Init()
{
	m_x = 0;
	m_y = 0;
}

void CCamera::SetCenterX(int x)
{
	if (x - (WINCX / 2) < 0) m_x = 0;
	else if (x - (WINCX / 2) > m_maxX) m_x = m_maxX;
	else m_x = x - (WINCX / 2);
}

void CCamera::SetCenterY(int y)
{
	if (y - (WINCY / 2) < 0) m_y = 0;
	else if (y - (WINCY / 2) > m_maxY) m_y = m_maxY;
	else m_y = y - (WINCY / 2);
}
