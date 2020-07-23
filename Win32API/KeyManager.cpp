#include "stdafx.h"
#include "KeyManager.h"

CKeyManager* CKeyManager::m_instance = nullptr;

CKeyManager::CKeyManager()
{
}


CKeyManager::~CKeyManager()
{
}

CKeyManager * CKeyManager::GetInstance()
{
	if (nullptr == m_instance) m_instance = new CKeyManager;
	return m_instance;
}

void CKeyManager::DestroyInstance()
{
	if (nullptr != m_instance)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

void CKeyManager::Update()
{
	m_key = 0;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) m_key |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) m_key |= KEY_RBUTTON;
	if (GetAsyncKeyState('1') & 0x8000) m_key |= KEY_1;
	if (GetAsyncKeyState('2') & 0x8000) m_key |= KEY_2;
	if (GetAsyncKeyState('3') & 0x8000) m_key |= KEY_3;
	if (GetAsyncKeyState('4') & 0x8000) m_key |= KEY_4;
	if (GetAsyncKeyState('5') & 0x8000) m_key |= KEY_5;
	if (GetAsyncKeyState('6') & 0x8000) m_key |= KEY_6;
	if (GetAsyncKeyState(VK_UP) & 0x8000) m_key |= KEY_UP;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) m_key |= KEY_LEFT;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) m_key |= KEY_DOWN;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) m_key |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_BACK) & 0x8000) m_key |= KEY_BACK;
	if (GetAsyncKeyState('R') & 0x8000) m_key |= KEY_R;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) m_key |= KEY_SPACE;
	if (GetAsyncKeyState('Z') & 0x8000) m_key |= KEY_Z;
	if (GetAsyncKeyState('X') & 0x8000) m_key |= KEY_X;
}

bool CKeyManager::isKeyDown(DWORD key)
{
	if ((m_key & key) && !(m_keyDown & key))
	{
		m_keyDown |= key;
		return true;
	}
	else if (!(m_key & key) && (m_keyDown & key))
	{
		m_keyDown ^= key;
		return false;
	}
	return false;
}

bool CKeyManager::isKeyUp(DWORD key)
{
	if (m_key & key)
	{
		m_keyUp |= key;
		return false;
	}
	else if (m_keyUp & key)
	{
		m_keyUp ^= key;
		return true;
	}

	return false;
}

bool CKeyManager::isKeyPressing(DWORD key)
{
	if (m_key & key) return true;
	return false;
}
