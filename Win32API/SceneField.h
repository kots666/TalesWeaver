#pragma once
#include "Scene.h"
class CSceneField :
	public CScene
{
public:
	explicit CSceneField();
	virtual ~CSceneField();

public:
	// CScene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	bool m_isSpawn;
};

