#include "stdafx.h"
#include "MainApp.h"
#include "Mouse.h"

CMainApp::CMainApp() : m_sceneMgr(CSceneManager::GetInstance()), m_frameTime(GetTickCount()), m_FPS(0)
{
	Init();
}

CMainApp::~CMainApp()
{
	Release();
}

void CMainApp::Init()
{
	LoadAllImage();

	m_hDC = GetDC(g_hWnd);

	CObj* newMouse = new CMouse;

	CObjManager::GetInstance()->AddObject(newMouse, OBJ::MOUSE);
	CSoundManager::GetInstance();
	CInvenManager::GetInstance();
	CEquipManager::GetInstance();
}

void CMainApp::Update()
{
	CKeyManager::GetInstance()->Update();
	m_sceneMgr->Update();
}

void CMainApp::LateUpdate()
{
	CCollisionManager::EnemyCollision_Mul(*CObjManager::GetInstance()->GetList(OBJ::MONSTER), *CObjManager::GetInstance()->GetList(OBJ::PLAYER_SKILL));
	CCollisionManager::EnemyCollision(*CObjManager::GetInstance()->GetList(OBJ::MONSTER), *CObjManager::GetInstance()->GetList(OBJ::PLAYER_ATTACK));
	CCollisionManager::PlayerCollision_Mul(CObjManager::GetInstance()->GetPlayer(), *CObjManager::GetInstance()->GetList(OBJ::ENEMY_ATTACK_MUL));
	CCollisionManager::PlayerCollision(CObjManager::GetInstance()->GetPlayer(), *CObjManager::GetInstance()->GetList(OBJ::ENEMY_ATTACK_ONE));
	m_sceneMgr->LateUpdate();
}

void CMainApp::Render()
{
	HDC backDC = CBitmapManager::GetInstance()->GetDC(__T("BackBuffer"));
	if (nullptr == backDC) return;
	HDC memDC = CBitmapManager::GetInstance()->GetDC(__T("Back"));
	if (nullptr == memDC) return;

	BitBlt(backDC, 0, 0, WINCX, WINCY, memDC, 0, 0, SRCCOPY);

	HFONT myFont = CreateFont(11, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, __T("±¼¸²"));
	HFONT oldFont = (HFONT)SelectObject(backDC, myFont);

	SetBkMode(backDC, 1);
	SetTextColor(backDC, RGB(255, 255, 255));

	m_sceneMgr->Render(backDC);

	SelectObject(backDC, oldFont);
	DeleteObject(myFont);

	BitBlt(m_hDC, 0, 0, WINCX, WINCY, backDC, 0, 0, SRCCOPY);

	++m_FPS;
	if (m_frameTime + 1000 < GetTickCount())
	{
		TCHAR nameBuffer[32] = __T("");
		wsprintf(nameBuffer, __T("TalesWeaver  FPS : %d"), m_FPS);
		SetWindowText(g_hWnd, nameBuffer);

		m_frameTime = GetTickCount();
		m_FPS = 0;
	}
}

void CMainApp::Release()
{
	ReleaseDC(g_hWnd, m_hDC);

	CSceneManager::DestroyInstance();
	CObjManager::DestroyInstance();
	CKeyManager::DestroyInstance();
	CBitmapManager::DestroyInstance();
	CTileManager::DestroyInstance();
	CSoundManager::DestroyInstance();
	CInvenManager::DestroyInstance();
	CEquipManager::DestroyInstance();
}

void CMainApp::LoadAllImage()
{
	// BackBuffer
	CBitmapManager::GetInstance()->InsertBitmap(__T("Back"), __T("../Image/Back.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("BackBuffer"), __T("../Image/BackBuffer.bmp"));

	// Logo & Title
	CBitmapManager::GetInstance()->InsertBitmap(__T("Logo"), __T("../Image/Logo/Logo.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Title"), __T("../Image/Title/Title.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Start"), __T("../Image/Title/Start.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Credit"), __T("../Image/Title/Credit.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Exit"), __T("../Image/Title/Exit.bmp"));

	// MiniMap
	CBitmapManager::GetInstance()->InsertBitmap(__T("Mini_Town"), __T("../Image/Map/Mini_Town.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Mini_Shop"), __T("../Image/Map/Mini_Shop.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Mini_Field"), __T("../Image/Map/Mini_Field.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Mini_Dun1"), __T("../Image/Map/Mini_Dun1.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Mini_Dun2"), __T("../Image/Map/Mini_Dun2.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Mini_Boss"), __T("../Image/Map/Mini_Boss.bmp"));

	// Map
	CBitmapManager::GetInstance()->InsertBitmap(__T("Town"), __T("../Image/Map/Town.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Shop"), __T("../Image/Map/Shop.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Field"), __T("../Image/Map/Field.bmp"));
	//CBitmapManager::GetInstance()->InsertBitmap(__T("Dun1"), __T("../Image/Map/Dun1.bmp"));
	//CBitmapManager::GetInstance()->InsertBitmap(__T("Dun2"), __T("../Image/Map/Dun2.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Boss"), __T("../Image/Map/Boss.bmp"));

	// Mouse
	CBitmapManager::GetInstance()->InsertBitmap(__T("Mouse_Idle"), __T("../Image/UI/Mouse/Mouse_Idle.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Mouse_Attack"), __T("../Image/UI/Mouse/Mouse_Attack.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Mouse_Fail"), __T("../Image/UI/Mouse/Mouse_Fail.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Mouse_Portal"), __T("../Image/UI/Mouse/Mouse_Portal.bmp"));

	// Tile
	CBitmapManager::GetInstance()->InsertBitmap(__T("Tile_Edit"), __T("../Image/Tile/Edit.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Tile_Normal"), __T("../Image/Tile/tile_0.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Tile_Fail"), __T("../Image/Tile/tile_1.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Tile_Town"), __T("../Image/Tile/tile_2.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Tile_Shop"), __T("../Image/Tile/tile_3.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Tile_Field"), __T("../Image/Tile/tile_4.bmp"));
	//CBitmapManager::GetInstance()->InsertBitmap(__T("Tile_Dun1"), __T("../Image/Tile/tile_5.bmp"));
	//CBitmapManager::GetInstance()->InsertBitmap(__T("Tile_Dun2"), __T("../Image/Tile/tile_6.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Tile_Boss"), __T("../Image/Tile/tile_7.bmp"));

	// NPC
	CBitmapManager::GetInstance()->InsertBitmap(__T("NPC"), __T("../Image/NPC/NPC.bmp"));

	// Player
	CBitmapManager::GetInstance()->InsertBitmap(__T("Player_Left"), __T("../Image/Player/Player_Left.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Player_LD"), __T("../Image/Player/Player_LD.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Player_LU"), __T("../Image/Player/Player_LU.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Player_Right"), __T("../Image/Player/Player_Right.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Player_RD"), __T("../Image/Player/Player_RD.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Player_RU"), __T("../Image/Player/Player_RU.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Player_Down"), __T("../Image/Player/Player_Down.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Player_Up"), __T("../Image/Player/Player_Up.bmp"));

	// Skill
	CBitmapManager::GetInstance()->InsertBitmap(__T("Skill_Moon"), __T("../Image/Skill/Skill_Moon.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Skill_Boom"), __T("../Image/Skill/Skill_Boom.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Skill_Soul"), __T("../Image/Skill/Skill_Soul.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Skill_Multi"), __T("../Image/Skill/Skill_Multi.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Skill_Quake"), __T("../Image/Skill/Skill_Quake.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Skill_Lightning"), __T("../Image/Skill/Skill_Lightning.bmp"));

	// Monster
	CBitmapManager::GetInstance()->InsertBitmap(__T("Jelly_LD"), __T("../Image/Monster/Jelly_LD.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Jelly_LU"), __T("../Image/Monster/Jelly_LU.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Jelly_RD"), __T("../Image/Monster/Jelly_RD.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Jelly_RU"), __T("../Image/Monster/Jelly_RU.bmp"));

	// Boss
	CBitmapManager::GetInstance()->InsertBitmap(__T("Boss_Down"), __T("../Image/Boss/Boss_Down.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Boss_Left"), __T("../Image/Boss/Boss_Left.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Boss_Right"), __T("../Image/Boss/Boss_Right.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Boss_Up"), __T("../Image/Boss/Boss_Up.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Boss_LD"), __T("../Image/Boss/Boss_LD.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Boss_LU"), __T("../Image/Boss/Boss_LU.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Boss_RD"), __T("../Image/Boss/Boss_RD.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Boss_RU"), __T("../Image/Boss/Boss_RU.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Boss_UI"), __T("../Image/Boss/Boss_UI.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Boss_HP"), __T("../Image/Boss/Boss_Hp.bmp"));

	// EFFECT
	CBitmapManager::GetInstance()->InsertBitmap(__T("Portal"), __T("../Image/Effect/Portal.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Crash"), __T("../Image/Effect/Crash.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Crash2"), __T("../Image/Effect/Crash2.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Number"), __T("../Image/Effect/Number.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Combo"), __T("../Image/Effect/Combo.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Comline"), __T("../Image/Effect/Comline.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("LvUp"), __T("../Image/Effect/LvUp.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Miss"), __T("../Image/Effect/Miss.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("HpPotion"), __T("../Image/Effect/HpPotion.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("MpPotion"), __T("../Image/Effect/MpPotion.bmp"));

	// UI
	CBitmapManager::GetInstance()->InsertBitmap(__T("Small_Hp"), __T("../Image/UI/HP/Small_Hp.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Small_UI"), __T("../Image/UI/HP/Small_UI.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Player_Info"), __T("../Image/UI/Player_Info.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Player_Hp"), __T("../Image/UI/Player_Hp.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Player_Mp"), __T("../Image/UI/Player_Mp.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Player_Sp"), __T("../Image/UI/Player_Sp.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Player_MoveState"), __T("../Image/UI/Player_MoveState.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Player_Slot"), __T("../Image/UI/Player_Slot.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Skill_Icon"), __T("../Image/UI/Skill_Icon.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Stat_Button"), __T("../Image/UI/Stat_Button.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Equip_Button"), __T("../Image/UI/Equip_Button.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Inven_Button"), __T("../Image/UI/Inven_Button.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Stat_Info"), __T("../Image/UI/Stat_Info.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Equip_Info"), __T("../Image/UI/Equip_Info.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Inven_Info"), __T("../Image/UI/Inven_Info.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Exp"), __T("../Image/UI/Exp.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Episode"), __T("../Image/UI/Episode.bmp"));

	// Shop
	CBitmapManager::GetInstance()->InsertBitmap(__T("ShopWnd"), __T("../Image/Shop/ShopWnd.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Buy"), __T("../Image/Shop/Buy.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Sell"), __T("../Image/Shop/Sell.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("OK"), __T("../Image/Shop/OK.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("Cancle"), __T("../Image/Shop/Cancle.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("WndClose"), __T("../Image/Shop/WndClose.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("WndUp"), __T("../Image/Shop/WndUp.bmp"));
	CBitmapManager::GetInstance()->InsertBitmap(__T("WndDown"), __T("../Image/Shop/WndDown.bmp"));

	// Item
	CBitmapManager::GetInstance()->InsertBitmap(__T("Items"), __T("../Image/Item/Items.bmp"));
}
