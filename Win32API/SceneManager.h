#pragma once

class CScene;

class CSceneManager
{
public:
	enum INDEX { Scene_Logo, Scene_Title, Scene_Edit, Scene_Town, Scene_Shop, Scene_Field, Scene_Boss, Scene_End };

private:
	CSceneManager();
	~CSceneManager();

public:
	static CSceneManager* GetInstance();
	static void DestroyInstance();

public:
	void Init();
	void Update();
	void LateUpdate();
	void Render(HDC hDC);
	void Release();

	void ChangeScene(INDEX scene, int option);

private:
	static CSceneManager* m_instance;
	CScene* m_scene;
	INDEX m_curScene;
	INDEX m_nextScene;
};

