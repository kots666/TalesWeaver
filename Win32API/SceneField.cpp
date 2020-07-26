#include "stdafx.h"
#include "SceneField.h"
#include "Player.h"
#include "Effect.h"
#include "Jelly.h"

CSceneField::CSceneField() : m_isSpawn(false)
{
	Init();
}

CSceneField::~CSceneField()
{
	Release();
}

void CSceneField::Init()
{
	CTileManager::GetInstance()->LoadData(EDIT::FIELD);

	CCamera::SetMaxX(3180 - WINCX);
	CCamera::SetMaxY(1687 - WINCY);

	CObjManager::GetInstance()->GetPlayer()->SetXY(1450.f, 1000.f);
	CCamera::SetCenterX(1450);
	CCamera::SetCenterY(1000);

	// 필드 → 타운
	CObj* newPortal = new CEffect(1625.f, 840.f, 150, 200, __T("Portal"), 0, 24, 50, RGB(255, 255, 255));
	CObjManager::GetInstance()->AddObject(newPortal, OBJ::EFFECT);

	// 임시로 몬스터 생성
	CObj* newMonster = new CJelly(825.f, 400.f);
	CObjManager::GetInstance()->AddObject(newMonster, OBJ::MONSTER);

	newMonster = new CJelly(825.f, 1275.f);
	CObjManager::GetInstance()->AddObject(newMonster, OBJ::MONSTER);

	CSoundManager::GetInstance()->PlayBGM(__T("BGM_Field.mp3"));
	CSoundManager::GetInstance()->SetVolume(0.3f, CSoundManager::BGM);
}

void CSceneField::Update()
{
	int monsterNum = CObjManager::GetInstance()->GetList(OBJ::MONSTER)->size();
	if (!monsterNum && !m_isSpawn)
	{
		// 필드 → 보스
		CObj* newPortal = new CEffect(850.f, 450.f, 150, 200, __T("Portal"), 0, 24, 50, RGB(255, 255, 255));
		CObjManager::GetInstance()->AddObject(newPortal, OBJ::EFFECT);

		m_isSpawn = true;
	}
}

void CSceneField::LateUpdate()
{
	INFO playerInfo = CObjManager::GetInstance()->GetPlayer()->GetInfo();

	int x = playerInfo.xPos / TILECX;
	int y = playerInfo.yPos / TILECY;
	int index = x + y * TILEX;
	if (TILE_TOWN == CCollisionManager::GetCollideTileState(index)) CSceneManager::GetInstance()->ChangeScene(CSceneManager::Scene_Town, FIELD_TO_TOWN);
	if (TILE_BOSS == CCollisionManager::GetCollideTileState(index))
	{
		int monsterNum = CObjManager::GetInstance()->GetList(OBJ::MONSTER)->size();
		if(!monsterNum)	CSceneManager::GetInstance()->ChangeScene(CSceneManager::Scene_Boss, NULL);
	}
}

void CSceneField::Render(HDC hDC)
{
	HDC memDC = CBitmapManager::GetInstance()->GetDC(__T("Field"));

	BitBlt(hDC, 0, 0, WINCX, WINCY, memDC, CCamera::GetX(), CCamera::GetY(), SRCCOPY);

	//CTileManager::GetInstance()->Render(hDC);
}

void CSceneField::Release()
{
	CObjManager::GetInstance()->RemoveObject(OBJ::EFFECT);
	CObjManager::GetInstance()->RemoveObject(OBJ::MONSTER);
	CSoundManager::GetInstance()->StopSound(CSoundManager::BGM);
}
