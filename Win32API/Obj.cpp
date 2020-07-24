#include "stdafx.h"
#include "Obj.h"


CObj::CObj() : m_speed(0.f), m_isDead(false)
{
	ZeroMemory(&m_rect, sizeof(RECT));
	ZeroMemory(&m_info, sizeof(INFO));
	ZeroMemory(&m_frame, sizeof(FRAME));
}


CObj::~CObj()
{
}

void CObj::UpdateRect()
{
	m_rect.left = LONG(m_info.xPos - (m_info.xSize >> 1));
	m_rect.top = LONG(m_info.yPos - (m_info.ySize >> 1));
	m_rect.right = LONG(m_info.xPos + (m_info.xSize >> 1));
	m_rect.bottom = LONG(m_info.yPos + (m_info.ySize >> 1));
}

int CObj::UpdateFrame()
{
	if (m_frame.frameTime + m_frame.frameSpeed < GetTickCount())
	{
		m_frame.frameTime = GetTickCount();
		++m_frame.startFrame;
	}

	if (m_frame.startFrame >= m_frame.endFrame)
	{
		m_frame.startFrame = 0;
		return FRAME_END;
	}

	return FRAME_NO_EVENT;
}
