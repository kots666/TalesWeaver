#pragma once
#include "Scene.h"
class CSceneTown :
	public CScene
{
public:
	explicit CSceneTown(int sceneCase);
	virtual ~CSceneTown();

public:
	// CScene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
};

