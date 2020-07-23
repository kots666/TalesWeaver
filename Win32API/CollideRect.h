#pragma once
#include "Obj.h"
class CCollideRect :
	public CObj
{
public:
	CCollideRect(float x, float y, int cx, int cy, int damage, DWORD lifeTime = 50);
	virtual ~CCollideRect();

public:
	// CObj��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void SetIsHit() { m_isHit = true; }

private:
	DWORD m_spawnTime;
	DWORD m_lifeTime;
	bool m_isHit;
};

