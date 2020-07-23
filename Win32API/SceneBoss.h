#pragma once
#include "Scene.h"
class CSceneBoss :
	public CScene
{
public:
	explicit CSceneBoss();
	virtual ~CSceneBoss();

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

