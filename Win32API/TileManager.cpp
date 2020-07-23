#include "stdafx.h"
#include "TileManager.h"
#include "Tile.h"

CTileManager* CTileManager::m_instance = nullptr;

CTileManager::CTileManager()
{
	m_tileVec.reserve(TILEX * TILEY);
	Init();
}


CTileManager::~CTileManager()
{
	Release();
}

CTileManager * CTileManager::GetInstance()
{
	if (nullptr == m_instance) m_instance = new CTileManager;
	return m_instance;
}

void CTileManager::DestroyInstance()
{
	if (nullptr != m_instance)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

void CTileManager::Init()
{
	for (int y = 0; y < TILEY; ++y)
	{
		float yPos = y * TILECY + (TILECY >> 1);
		for (int x = 0; x < TILEX; ++x)
		{
			float xPos = x * TILECX + (TILECX >> 1);
			CTile* newTile = new CTile(xPos, yPos, TILE_FAIL);
			m_tileVec.emplace_back(newTile);
		}
	}
}

void CTileManager::ChangeTile(int index, int state)
{
	m_tileVec[index]->SetState(state);
}

void CTileManager::SaveData(EDIT::STATE map)
{
	TCHAR* path = nullptr;

	switch (map)
	{
	case EDIT::TOWN: path = __T("../Data/Town.dat"); break;
	case EDIT::SHOP: path = __T("../Data/Shop.dat"); break;
	case EDIT::FIELD: path = __T("../Data/Field.dat"); break;
	//case EDIT::DUN1: path = __T("../Data/Dun1.dat"); break;
	//case EDIT::DUN2: path = __T("../Data/Dun2.dat"); break;
	case EDIT::BOSS: path = __T("../Data/Boss.dat"); break;
	}

	if (nullptr == path) return;

	HANDLE file = CreateFile(path, GENERIC_WRITE, NULL, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == file)
	{
		MessageBox(nullptr, __T("Error"), __T("Save Fail"), MB_OK);
		return;
	}

	DWORD bytes = 0;

	for (auto& elem : m_tileVec)
	{
		int state = elem->GetState();
		WriteFile(file, &state, sizeof(int), &bytes, nullptr);
	}

	CloseHandle(file);
}

void CTileManager::LoadData(EDIT::STATE map)
{
	TCHAR* path = nullptr;

	switch (map)
	{
	case EDIT::TOWN: path = __T("../Data/Town.dat"); break;
	case EDIT::SHOP: path = __T("../Data/Shop.dat"); break;
	case EDIT::FIELD: path = __T("../Data/Field.dat"); break;
		//case EDIT::DUN1: path = __T("../Data/Dun1.dat"); break;
		//case EDIT::DUN2: path = __T("../Data/Dun2.dat"); break;
	case EDIT::BOSS: path = __T("../Data/Boss.dat"); break;
	}

	if (nullptr == path) return;

	HANDLE file = CreateFile(path, GENERIC_READ, NULL, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == file)
	{
		MessageBox(nullptr, __T("Error"), __T("Load Fail"), MB_OK);
		return;
	}

	DWORD bytes = 0;
	int state = 0;
	int index = 0;

	while (true)
	{
		ReadFile(file, &state, sizeof(int), &bytes, nullptr);
		if (0 == bytes || index >= m_tileVec.size()) break;

		m_tileVec[index++]->SetState(state);
	}

	CloseHandle(file);
}

CTile * CTileManager::GetTile(int index)
{
	if (0 > index || index >= m_tileVec.size()) return nullptr;
	return m_tileVec[index];
}

void CTileManager::Render(HDC hDC)
{
	int startX = CCamera::GetX() / TILECX;
	int startY = CCamera::GetY() / TILECY;

	int endX = startX + WINCX / TILECX;
	int endY = startY + WINCY / TILECY;

	for (int y = startY; y <= endY; ++y)
	{
		for (int x = startX; x <= endX; ++x)
		{
			int index = x + (TILEX * y);

			if (0 > index || index >= m_tileVec.size()) continue;

			m_tileVec[index]->Render(hDC);
		}
	}
}

void CTileManager::Release()
{
	for (auto& elem : m_tileVec)
	{
		SafeDelete(elem);
	}
	m_tileVec.clear();
	m_tileVec.shrink_to_fit();
}
