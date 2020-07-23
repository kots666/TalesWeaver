#include "stdafx.h"
#include "SceneTown.h"
#include "Player.h"
#include "Effect.h"

CSceneTown::CSceneTown(int sceneCase)
{
	if (TITLE_TO_TOWN == sceneCase)
	{
		CObj* player = new CPlayer(875.f, 575.f);
		CObjManager::GetInstance()->AddObject(player, OBJ::PLAYER);
	}
	else if (SHOP_TO_TOWN == sceneCase)
	{
		CObjManager::GetInstance()->GetPlayer()->SetXY(325.f, 495.f);
		CCamera::SetCenterX(325);
		CCamera::SetCenterY(495);
	}
	else if (FIELD_TO_TOWN == sceneCase)
	{
		CObjManager::GetInstance()->GetPlayer()->SetXY(775.f, 825.f);
		CCamera::SetCenterX(775);
		CCamera::SetCenterY(825);
	}
	Init();
}

CSceneTown::~CSceneTown()
{
	Release();
}

void CSceneTown::Init()
{
	CTileManager::GetInstance()->LoadData(EDIT::TOWN);

	CCamera::SetMaxX(1600 - WINCX);
	CCamera::SetMaxY(1200 - WINCY);

	//상점 포탈 위치는 index x : 8 y : 9
	CObj* newPortal = new CEffect(400.f, 450.f, 150, 200, __T("Portal"), 0, 24, 50, RGB(255,255,255));
	CObjManager::GetInstance()->AddObject(newPortal, OBJ::EFFECT);

	//필드 포탈 위치는 x : 14 y : 17
	newPortal = new CEffect(700.f, 850.f, 150, 200, __T("Portal"), 0, 24, 50, RGB(255, 255, 255));
	CObjManager::GetInstance()->AddObject(newPortal, OBJ::EFFECT);
}

void CSceneTown::Update()
{
	
}

void CSceneTown::LateUpdate()
{
	INFO playerInfo = CObjManager::GetInstance()->GetPlayer()->GetInfo();

	int x = playerInfo.xPos / TILECX;
	int y = playerInfo.yPos / TILECY;
	int index = x + y * TILEX;
	if (TILE_SHOP == CCollisionManager::GetCollideTileState(index)) CSceneManager::GetInstance()->ChangeScene(CSceneManager::Scene_Shop, NULL);
	if (TILE_FIELD == CCollisionManager::GetCollideTileState(index)) CSceneManager::GetInstance()->ChangeScene(CSceneManager::Scene_Field, NULL);
}

void CSceneTown::Render(HDC hDC)
{
	HDC memDC = CBitmapManager::GetInstance()->GetDC(__T("Town"));

	BitBlt(hDC, 0, 0, WINCX, WINCY, memDC, CCamera::GetX(), CCamera::GetY(), SRCCOPY);

	CTileManager::GetInstance()->Render(hDC);
}

void CSceneTown::Release()
{
	CObjManager::GetInstance()->RemoveObject(OBJ::EFFECT);
}
