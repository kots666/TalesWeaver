#pragma once
#include "Obj.h"
class CTargetUI :
	public CObj
{
public:
	explicit CTargetUI(CObj* target, int cx, int cy, const TCHAR* key, int width, int height, int scene, int end, DWORD frameSpeed, DWORD transColor = RGB(255, 0, 255), bool isOnce = false, int offsetX = 3, int offsetY = 70);
	virtual ~CTargetUI();

public:
	// CObj을(를) 통해 상속됨
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	CObj* m_target;
	const TCHAR* m_key;
	DWORD m_color;
	int m_width;
	int m_height;
	int m_offsetX;
	int m_offsetY;
	bool m_isOnce;
};

