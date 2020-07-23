#pragma once
#include "Obj.h"
class CDamageFont :
	public CObj
{
public:
	explicit CDamageFont(float x, float y, int id);
	virtual ~CDamageFont();

	// CObj��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

private:
	int m_ID;
	DWORD m_spawnTime;
};

