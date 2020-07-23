#pragma once
class CKeyManager
{
private:
	CKeyManager();
	~CKeyManager();

public:
	static CKeyManager* GetInstance();
	static void DestroyInstance();

public:
	void Update();

public:
	bool isKeyDown(DWORD key);
	bool isKeyUp(DWORD key);
	bool isKeyPressing(DWORD key);

private:
	static CKeyManager* m_instance;
	DWORD m_key;
	DWORD m_keyUp;
	DWORD m_keyDown;
};

