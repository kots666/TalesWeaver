#pragma once
#include "Obj.h"
class CInfoButton :
	public CObj
{
public:
	explicit CInfoButton(float x, float y, int cx, int cy, const TCHAR* key, int cx2, int cy2, const TCHAR* child);
	virtual ~CInfoButton();

public:
	// CObj을(를) 통해 상속됨
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	const TCHAR* m_key;
	const TCHAR* m_childKey;
	int m_childCX;
	int m_childCY;
	bool m_isChildOpen;
	int m_ID;
};

