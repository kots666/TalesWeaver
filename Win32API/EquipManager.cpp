#include "stdafx.h"
#include "EquipManager.h"
#include "Item.h"
#include "Player.h"

CEquipManager* CEquipManager::m_instance = nullptr;

CEquipManager::CEquipManager()
{
	m_head = nullptr;
	m_armor = nullptr;
	m_arms = nullptr;
	m_hand = nullptr;
	m_foot = nullptr;
}

CEquipManager::~CEquipManager()
{
	ReleaseItem(m_head);
	ReleaseItem(m_armor);
	ReleaseItem(m_arms);
	ReleaseItem(m_hand);
	ReleaseItem(m_foot);
}

CEquipManager * CEquipManager::GetInstance()
{
	if (nullptr == m_instance) m_instance = new CEquipManager;

	return m_instance;
}

void CEquipManager::DestroyInstance()
{
	if (nullptr != m_instance)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

void CEquipManager::Refresh()
{
	CPlayer* player = dynamic_cast<CPlayer*>(CObjManager::GetInstance()->GetPlayer());
	if (nullptr == player) return;

	player->RefreshStat();
}

void CEquipManager::EquipItem(int ID)
{
	if (0 <= ID && ID <= 2) return;
	if (13 == ID) return;

	if (3 <= ID && ID <= 4) EquipAndReturn(m_armor, ID);
	if (5 <= ID && ID <= 6) EquipAndReturn(m_arms, ID);
	if (7 <= ID && ID <= 8) EquipAndReturn(m_foot, ID);
	if (9 <= ID && ID <= 10) EquipAndReturn(m_hand, ID);
	if (11 <= ID && ID <= 12) EquipAndReturn(m_head, ID);
}

void CEquipManager::UnequipItem(int ID)
{
	CItem** target = nullptr;

	switch (ID)
	{
	case 0: target = &m_head; break;
	case 1: target = &m_armor; break;
	case 2: target = &m_arms; break;
	case 3: target = &m_hand; break;
	case 4: target = &m_foot; break;
	}

	if (nullptr != *target)
	{
		CItem* newItem = new CItem((*target)->GetID());
		CInvenManager::GetInstance()->AddItem(newItem);

		ReleaseItem(*target);
	}

	Refresh();
}

void CEquipManager::ReleaseItem(CItem *& item)
{
	if (nullptr != item)
	{
		delete item;
		item = nullptr;
	}
}

void CEquipManager::EquipAndReturn(CItem *& item, int ID)
{
	if(nullptr != item)
	{
		CItem* returnItem = new CItem(item->GetID());
		CInvenManager::GetInstance()->ReturnItem(returnItem);

		ReleaseItem(item);
	}

	item = new CItem(ID);

	Refresh();
}

bool CEquipManager::CanEquip(int ID)
{
	if (0 == ID || 1 == ID || 2 == ID || 13 == ID) return false;
	return true;
}
