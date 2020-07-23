#pragma once
#include "Obj.h"
class CComboUI :
	public CObj
{
public:
	explicit CComboUI(CObj* target);
	virtual ~CComboUI();

public:
	// CObj을(를) 통해 상속됨
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void RenderCombo(HDC hDC, float x, float y);

private:
	CObj * m_target;
};

