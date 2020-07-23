#pragma once
#include "Obj.h"
class CComboLine :
	public CObj
{
public:
	explicit CComboLine();
	virtual ~CComboLine();

public:
	// CObj을(를) 통해 상속됨
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	DWORD m_spawnTime;
};

