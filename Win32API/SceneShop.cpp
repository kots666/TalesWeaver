#include "stdafx.h"
#include "SceneShop.h"
#include "Player.h"
#include "Effect.h"
#include "NPC.h"

CSceneShop::CSceneShop()
{
	Init();
}

CSceneShop::~CSceneShop()
{
	Release();
}

void CSceneShop::Init()
{
	CTileManager::GetInstance()->LoadData(EDIT::SHOP);

	CCamera::SetMaxX(1277 - WINCX);
	CCamera::SetMaxY(991 - WINCY);

	CObjManager::GetInstance()->GetPlayer()->SetXY(925.f, 775.f);
	CCamera::SetCenterX(925);
	CCamera::SetCenterY(775);

	CObj* newPortal = new CEffect(975.f, 825.f, 150, 200, __T("Portal"), 0, 24, 50, RGB(255, 255, 255));
	CObjManager::GetInstance()->AddObject(newPortal, OBJ::EFFECT);

	CObj* newNPC = new CNPC(535.f, 430.f, 96, 92, 1);
	CObjManager::GetInstance()->AddObject(newNPC, OBJ::EFFECT);

	CSoundManager::GetInstance()->PlayBGM(__T("BGM_Store.wav"));
}

/*
	x : 19
	y : 16
*/

void CSceneShop::Update()
{
}

void CSceneShop::LateUpdate()
{
	INFO playerInfo = CObjManager::GetInstance()->GetPlayer()->GetInfo();

	int x = playerInfo.xPos / TILECX;
	int y = playerInfo.yPos / TILECY;
	int index = x + y * TILEX;
	if (TILE_TOWN == CCollisionManager::GetCollideTileState(index)) CSceneManager::GetInstance()->ChangeScene(CSceneManager::Scene_Town, SHOP_TO_TOWN);
}

void CSceneShop::Render(HDC hDC)
{
	HDC memDC = CBitmapManager::GetInstance()->GetDC(__T("Shop"));

	BitBlt(hDC, 0, 0, WINCX, WINCY, memDC, CCamera::GetX(), CCamera::GetY(), SRCCOPY);

	//CTileManager::GetInstance()->Render(hDC);
}

void CSceneShop::Release()
{
	CObjManager::GetInstance()->RemoveObject(OBJ::EFFECT);
	CObjManager::GetInstance()->RemoveObject(OBJ::SHOP);
	CSoundManager::GetInstance()->StopSound(CSoundManager::BGM);
}
