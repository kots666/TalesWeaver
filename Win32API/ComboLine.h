#pragma once
#include "Obj.h"
class CComboLine :
	public CObj
{
public:
	explicit CComboLine();
	virtual ~CComboLine();

public:
	// CObj��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	DWORD m_spawnTime;
};

