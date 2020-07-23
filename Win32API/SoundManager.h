#pragma once
class CSoundManager
{
private:
	CSoundManager();
	~CSoundManager();

public:
	static CSoundManager* GetInstance();
	static void DestroyInstance();

private:
	static CSoundManager* m_instance;
};

