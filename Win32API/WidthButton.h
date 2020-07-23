#pragma once
#include "Obj.h"
class CWidthButton :
	public CObj
{
public:
	CWidthButton(float x, float y, int cx, int cy, const TCHAR* key);
	virtual ~CWidthButton();

public:
	// CObj을(를) 통해 상속됨
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	const TCHAR* m_key;
	int m_ID;
};

