#include "stdafx.h"
#include "SceneBoss.h"
#include "Player.h"
#include "Boss.h"
#include "Effect.h"

CSceneBoss::CSceneBoss() : m_isSpawn(false)
{
	Init();
}

CSceneBoss::~CSceneBoss()
{
	Release();
}

void CSceneBoss::Init()
{
	CTileManager::GetInstance()->LoadData(EDIT::BOSS);

	CCamera::SetMaxX(3900 - WINCX);
	CCamera::SetMaxY(2040 - WINCY);

	CObjManager::GetInstance()->GetPlayer()->SetXY(1060.f, 760.f);
	CCamera::SetCenterX(1060);
	CCamera::SetCenterY(760);

	CObj* newBoss = new CBoss(2100.f, 1210.f);
	CObjManager::GetInstance()->AddObject(newBoss, OBJ::MONSTER);

	CSoundManager::GetInstance()->PlayBGM(__T("BGM_Boss.mp3"));
	CSoundManager::GetInstance()->SetVolume(0.5f, CSoundManager::BGM);
}

void CSceneBoss::Update()
{
	bool isBossLive = CObjManager::GetInstance()->GetList(OBJ::MONSTER)->size();
	if (!isBossLive && !m_isSpawn)
	{
		CObj* newPortal = new CEffect(1975.f, 1150.f, 150, 200, __T("Portal"), 0, 24, 50, RGB(255, 255, 255));
		CObjManager::GetInstance()->AddObject(newPortal, OBJ::EFFECT);
		m_isSpawn = true;

		CSoundManager::GetInstance()->PlayOverlapSound(__T("Success.wav"), CSoundManager::UI);
	}
}

void CSceneBoss::LateUpdate()
{
	INFO playerInfo = CObjManager::GetInstance()->GetPlayer()->GetInfo();

	int x = playerInfo.xPos / TILECX;
	int y = playerInfo.yPos / TILECY;
	int index = x + y * TILEX;

	if (TILE_TOWN == CCollisionManager::GetCollideTileState(index))
	{
		int monsterNum = CObjManager::GetInstance()->GetList(OBJ::MONSTER)->size();
		if (!monsterNum) CSceneManager::GetInstance()->ChangeScene(CSceneManager::Scene_End, NULL);
	}
}

void CSceneBoss::Render(HDC hDC)
{
	HDC memDC = CBitmapManager::GetInstance()->GetDC(__T("Boss"));

	BitBlt(hDC, 0, 0, WINCX, WINCY, memDC, CCamera::GetX(), CCamera::GetY(), SRCCOPY);

	//CTileManager::GetInstance()->Render(hDC);
}

void CSceneBoss::Release()
{
	CSoundManager::GetInstance()->StopSound(CSoundManager::BGM);
}
