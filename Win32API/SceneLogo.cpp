#include "stdafx.h"
#include "SceneLogo.h"
#include "MyBitmap.h"

CSceneLogo::CSceneLogo()
{
	Init();
}


CSceneLogo::~CSceneLogo()
{
	Release();
}

void CSceneLogo::Init()
{
	m_frame.startFrame = 0;
	m_frame.endFrame = 68;
	m_frame.sceneFrame = 0;
	m_frame.frameTime = GetTickCount();
	m_frame.frameSpeed = 50;
}

void CSceneLogo::Update()
{
	if (m_frame.frameTime + m_frame.frameSpeed < GetTickCount())
	{
		m_frame.frameTime = GetTickCount();
		++m_frame.startFrame;
	}
	// 로고가 끝나면 타이틀로 넘어간다.
	if (m_frame.startFrame >= m_frame.endFrame)
	{
		CSceneManager::GetInstance()->ChangeScene(CSceneManager::Scene_Title, NULL);
		return;
	}
}

void CSceneLogo::LateUpdate()
{
}

void CSceneLogo::Render(HDC hDC)
{
	HDC memDC = CBitmapManager::GetInstance()->GetDC(__T("Logo"));

	BitBlt(hDC, 0, 0, WINCX, WINCY, memDC, m_frame.startFrame * WINCX, m_frame.sceneFrame, SRCCOPY);
}

void CSceneLogo::Release()
{
}
