#include "stdafx.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SceneLogo.h"
#include "SceneTitle.h"
#include "SceneEdit.h"
#include "SceneTown.h"
#include "SceneShop.h"
#include "SceneField.h"
#include "SceneBoss.h"

CSceneManager* CSceneManager::m_instance = nullptr;

CSceneManager::CSceneManager()
{
	Init();
}

CSceneManager::~CSceneManager()
{
	Release();
}

CSceneManager * CSceneManager::GetInstance()
{
	if (nullptr == m_instance) m_instance = new CSceneManager;
	return m_instance;
}

void CSceneManager::DestroyInstance()
{
	if (nullptr != m_instance)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

void CSceneManager::Init()
{
	/*m_curScene = Scene_Logo;
	m_nextScene = m_curScene;
	m_scene = new CSceneLogo;*/

	m_curScene = Scene_Title;
	m_nextScene = m_curScene;
	m_scene = new CSceneTitle;
	
	/*m_curScene = Scene_Town;
	m_nextScene = m_curScene;
	m_scene = new CSceneTown(TITLE_TO_TOWN);*/
}

void CSceneManager::Update()
{
	if(nullptr != m_scene) m_scene->Update();
	CObjManager::GetInstance()->Update();
}

void CSceneManager::LateUpdate()
{
	if (nullptr != m_scene) m_scene->LateUpdate();
	CObjManager::GetInstance()->LateUpdate();
}

void CSceneManager::Render(HDC hDC)
{
	if (nullptr != m_scene) m_scene->Render(hDC);
	CObjManager::GetInstance()->Render(hDC);
}

void CSceneManager::Release()
{
	SafeDelete(m_scene);
}

void CSceneManager::ChangeScene(INDEX scene, int option)
{
	m_nextScene = scene;

	if (m_curScene != m_nextScene)
	{
		SafeDelete(m_scene);

		switch (m_nextScene)
		{
		case Scene_Logo:
			m_scene = new CSceneLogo;
			break;
		case Scene_Title:
			m_scene = new CSceneTitle;
			break;
		case Scene_Edit:
			m_scene = new CSceneEdit;
			break;
		case Scene_Town:
			m_scene = new CSceneTown(option);
			break;
		case Scene_Shop:
			m_scene = new CSceneShop;
			break;
		case Scene_Field:
			m_scene = new CSceneField;
			break;
		case Scene_Boss:
			m_scene = new CSceneBoss;
			break;
		case Scene_End:
			DestroyWindow(g_hWnd);
			return;
		}
		m_curScene = m_nextScene;
	}
}
