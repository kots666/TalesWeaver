#pragma once
#include "Obj.h"
class CEffect :
	public CObj
{
public:
	explicit CEffect(float x, float y, int cx, int cy, const TCHAR* key, int sceneFrame, int endFrame, DWORD frameSpeed, DWORD transColor, bool isOnce = false);
	virtual ~CEffect();

public:
	// CObj을(를) 통해 상속됨
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	const TCHAR* m_key;
	DWORD m_transColor;
	bool m_isOnce;
};

