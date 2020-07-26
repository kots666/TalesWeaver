#include "stdafx.h"
#include "InvenManager.h"
#include "Item.h"

CInvenManager* CInvenManager::m_instance = nullptr;

CInvenManager::CInvenManager()
{
	m_inventory.reserve(14);
}

CInvenManager::~CInvenManager()
{
	for (auto& elem : m_inventory)
	{
		delete elem;
		elem = nullptr;
	}

	m_inventory.clear();
	m_inventory.shrink_to_fit();
}

CInvenManager * CInvenManager::GetInstance()
{
	if (nullptr == m_instance) m_instance = new CInvenManager;

	return m_instance;
}

void CInvenManager::DestroyInstance()
{
	if (nullptr != m_instance)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

void CInvenManager::ReturnItem(CItem * item)
{
	int id = item->GetID();

	auto iter = find_if(m_inventory.begin(), m_inventory.end(), [&](CItem* item)
	{
		return id == item->GetID();
	});

	if (m_inventory.end() == iter)
	{
		AddItem(item);
	}
	else (*iter)->IncreaseCount();
}

void CInvenManager::IncreaseItem(int id)
{
	auto iter = find_if(m_inventory.begin(), m_inventory.end(), [&](CItem* item)
	{
		return id == item->GetID();
	});

	if (m_inventory.end() == iter) return;

	(*iter)->IncreaseCount();
}

void CInvenManager::DecreaseItem(int id)
{
	auto iter = find_if(m_inventory.begin(), m_inventory.end(), [&](CItem* item)
	{
		return id == item->GetID();
	});

	if (m_inventory.end() == iter) return;

	(*iter)->DecreaseCount();

	if (1 > (*iter)->GetCount())
	{
		SafeDelete(*iter);
		m_inventory.erase(iter);
	}
}

bool CInvenManager::IsItemExist(int id)
{
	auto iter = find_if(m_inventory.begin(), m_inventory.end(), [&](CItem* item)
	{
		return id == item->GetID();
	});

	if (m_inventory.end() == iter) return false;

	return true;
}
