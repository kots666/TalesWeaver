#pragma once
#include "Scene.h"
class CSceneBoss :
	public CScene
{
public:
	explicit CSceneBoss();
	virtual ~CSceneBoss();

public:
	// CScene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	bool m_isSpawn;
};

