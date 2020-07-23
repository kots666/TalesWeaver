#pragma once

class CSceneManager;

class CMainApp final
{
public:
	CMainApp();
	~CMainApp();

public:
	void Init();
	void Update();
	void LateUpdate();
	void Render();
	void Release();

	void LoadAllImage();

private:
	HDC m_hDC;
	CSceneManager* m_sceneMgr;
	DWORD m_frameTime;
	int m_FPS;
};

