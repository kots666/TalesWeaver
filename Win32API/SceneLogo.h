#pragma once
#include "Scene.h"

class CSceneLogo :
	public CScene
{
public:
	CSceneLogo();
	virtual ~CSceneLogo();

	// CScene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	FRAME m_frame;
};

