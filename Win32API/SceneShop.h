#pragma once
#include "Scene.h"
class CSceneShop :
	public CScene
{
public:
	explicit CSceneShop();
	virtual ~CSceneShop();

public:
	// CScene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
};

