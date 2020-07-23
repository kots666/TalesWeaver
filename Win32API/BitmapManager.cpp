#include "stdafx.h"
#include "BitmapManager.h"
#include "MyBitmap.h"

CBitmapManager* CBitmapManager::m_instance = nullptr;

CBitmapManager::CBitmapManager()
{
}


CBitmapManager::~CBitmapManager()
{
	Release();
}

CBitmapManager * CBitmapManager::GetInstance()
{
	if (nullptr == m_instance) m_instance = new CBitmapManager;
	return m_instance;
}

void CBitmapManager::DestroyInstance()
{
	if (nullptr != m_instance)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

void CBitmapManager::InsertBitmap(const TCHAR * key, const TCHAR * path)
{
	auto iter = find_if(m_bitmap.begin(), m_bitmap.end(), [&](auto& iter)
	{
		return (!lstrcmp(key, iter.first));
	});

	if (iter != m_bitmap.end()) return;

	CMyBitmap* newBitmap = new CMyBitmap;
	newBitmap->Load(path);
	m_bitmap.emplace(key, newBitmap);
}

CMyBitmap * CBitmapManager::GetMyBitmap(const TCHAR * target)
{
	auto iter = find_if(m_bitmap.begin(), m_bitmap.end(), [&](auto& iter)
	{
		return (!lstrcmp(target, iter.first));
	});

	if (iter == m_bitmap.end()) return nullptr;

	return iter->second;
}

HDC CBitmapManager::GetDC(const TCHAR * key)
{
	auto iter = find_if(m_bitmap.begin(), m_bitmap.end(), [&](auto& iter)
	{
		return (!lstrcmp(key, iter.first));
	});

	if (iter == m_bitmap.end()) return nullptr;

	return iter->second->GetMemDC();
}

void CBitmapManager::Release()
{
	for (auto& elem : m_bitmap)
	{
		SafeDelete(elem.second);
	}
	m_bitmap.clear();
}
