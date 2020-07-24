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

public:
	void StatRender(HDC hDC, float x, float y);
	void EquipRender(HDC hDC);
	void InvenRender(HDC hDC);

	void StatTextRender(HDC hDC, float x, float y);

private:
	const TCHAR* m_key;
	const TCHAR* m_childKey;
	int m_childCX;
	int m_childCY;
	bool m_isChildOpen;
	int m_ID;
};

