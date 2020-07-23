#include "stdafx.h"
#include "SoundManager.h"

CSoundManager* CSoundManager::m_instance = nullptr;

CSoundManager::CSoundManager()
{
}

CSoundManager::~CSoundManager()
{
}

CSoundManager * CSoundManager::GetInstance()
{
	if (nullptr == m_instance) m_instance = new CSoundManager;
	return m_instance;
}

void CSoundManager::DestroyInstance()
{
	if (nullptr != m_instance)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}
