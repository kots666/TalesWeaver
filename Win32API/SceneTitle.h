#pragma once
#include "Scene.h"
class CSceneTitle :
	public CScene
{
public:
	CSceneTitle();
	virtual ~CSceneTitle();

public:
	// CScene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
};

